#pragma once

#include "Game.h"

#include <vector>

class Node {
public:
	static constexpr double C_PARAM = 1.41; // sqrt(2)

	Node* selectBestChildUCT();
	Node* expand();	   // phase 2 - expansion
	Player simulate(); // phase 3 - simulation
	void backpropagate(Player winner);
	int visitCount() { return visits; }
	double winCount() { return wins; }
	const Game& getState() { return state; }
	std::vector<Node*> getChildren() { return children; }

	bool fullyExpanded();
	bool isLeaf();

	// constructor
	Node(Node* parent, const Game& state);
	~Node();
	// disable copy because copying tree is resource intensive
	Node(Node const&) = delete;
	void operator=(Node const&) = delete;

private:
	Node* parent;

	std::vector<Node*> children;
	std::vector<Play> unexpandedNodes;
	Game state;

	double wins = 0;
	int visits = 0;
};
