#include "pch.h"
#include "Node.h"

#include <algorithm>
#include <random>

using namespace std;

std::random_device r;
std::default_random_engine generator{ r() }; // random number generator

Node* Node::selectBestChildUCT() {
	Node* best = nullptr;
	double bestScore = -INFINITY;
	for (auto& child : children) {
		double score = (child->winCount() / child->visitCount()) +
					   C_PARAM * sqrt(log(winCount()) /
									  child->visitCount()); // UCB1 formula
		if (score > bestScore) {
			bestScore = score;
			best = child;
		}
	}
	return best;
}

Node* Node::expand() {
	Play play = unexpandedNodes[unexpandedNodes.size() - 1];
	unexpandedNodes.pop_back();
	// expand node
	Game state(this->state);
	state.applyMove(play);
	Node* node = new Node(this, state);
	// add to children vector
	children.push_back(node);
	return node;
}

Player Node::simulate() {
	// choose random moves until terminal
	Game tempState(this->state);

	while (!tempState.isTerminal()) {
		vector<Play> moves = tempState.moves();
		// pick random move
		uniform_int_distribution<int> distribution(0, moves.size() - 1);
		Play move = moves[distribution(generator)];
		tempState.applyMove(move); // advance state
	}
	return tempState.winner();
}

void Node::backpropagate(Player winner) {
	visits++;
	if ((state.playerToMove() == Player::X ? Player::O : Player::X) == winner) {
		wins++;
	}
	else if (winner == Player::None) {
		wins += 0.5;
	}
	// backpropagate parent
	if (parent != nullptr)
		parent->backpropagate(winner);
}

bool Node::fullyExpanded() { return unexpandedNodes.size() == 0; }

bool Node::isLeaf() { return state.isTerminal(); }

Node::Node(Node* parent, const Game& state) {
	this->parent = parent;
	this->state = state;

	// compute unexpanded nodes
	unexpandedNodes = state.moves();
	// shuffle
	shuffle(unexpandedNodes.begin(), unexpandedNodes.end(), generator);
}

Node::~Node() {
	// delete children
	for (size_t i = 0; i < children.size(); i++) {
		delete children[i];
	}
}
