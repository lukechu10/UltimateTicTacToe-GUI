#include "pch.h"
#include "MCTS.h"

#include <chrono>
#include <cmath>
#include <exception>
#include <stdexcept>

using namespace std;

Node* MCTS::traverse() {
	// start at tree root
	Node* node = root;
	while (node->fullyExpanded() && !node->isLeaf()) {
		Node* temp = node->selectBestChildUCT();
		if (temp == nullptr)
			break;
		else
			node = temp;
	}
	return node;
}

SearchResult MCTS::runSearch(double timeout) {
	// run 4 phases until no more time left
	int iterations = 0;
	auto timeStart = chrono::steady_clock::now();
	while (true) {
		// check if timeout exceeded
		auto timeNow = chrono::steady_clock::now();
		auto diff = timeNow - timeStart;
		if (chrono::duration<double, milli>(diff).count() > timeout)
			break; // timeout exceeded

		// run 100 simulations
		Node* node = traverse(); // phase 1 - selection
		if (node->fullyExpanded()) {
			Player winner = node->simulate();
			node->backpropagate(winner);
		} // backpropagate without expanding
		else {
			Node* expandedNode = node->expand();	  // phase 2 - expansion
			Player winner = expandedNode->simulate(); // phase 3 - simulation
			expandedNode->backpropagate(winner);
		}
		iterations++;
	}

	return SearchResult(iterations, root->winCount());
}

BestResult MCTS::bestMove(string policy) {
	// check if root is fully expanded
	if (!root->fullyExpanded()) {
		throw runtime_error(
			"Root is not fully expanded. There is not enough information");
	}
	// find best child
	int visitsBest = 0;
	double winsBest = 0;
	Play bestPlay;

	if (policy == "robust") {
		for (auto& child : root->getChildren()) {
			if (child->visitCount() > visitsBest) {
				visitsBest = child->visitCount();
				winsBest = child->winCount();
				bestPlay = child->getState().lastPlay();
			}
		}
	}
	else if (policy == "max") {
		double bestRatio = -INFINITY;
		for (auto& child : root->getChildren()) {
			double ratio = child->winCount() / child->visitCount();
			if (ratio > bestRatio) {
				bestRatio = ratio;
				visitsBest = child->visitCount();
				winsBest = child->winCount();
				bestPlay = child->getState().lastPlay();
			}
		}
	}

	return BestResult(visitsBest, winsBest, bestPlay);
}
