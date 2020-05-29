#pragma once
#include <array>
#include <iostream>
#include <vector>

#include "IGameState.h"

enum class Player : char {
	None = 0,
	X,
	O,
	Full // for winCache if quadrant has no more availible moves
};

/// <summary>
/// Converts a Player to its std::string representation
/// </summary>
/// <param name="p">Player to be converted</param>
/// <returns></returns>
std::string playerToString(const Player& p);
std::ostream& operator<<(std::ostream& os, const Player& p);

struct Play {
	unsigned int row = 0;
	unsigned int col = 0;
	unsigned int subRow = 0;
	unsigned int subCol = 0;
	Play() {}
	Play(unsigned int row, unsigned int col, unsigned int subRow,
		unsigned int subCol) {
		this->row = row;
		this->col = col;
		this->subRow = subRow;
		this->subCol = subCol;
	}
	bool operator==(const Play& p) const {
		return (row == p.row) && (col == p.col) && (subRow == p.subRow) &&
			   (subCol == p.subCol);
	}
};

class Game : IGameState<Play, Player> {
	using GameBoard =
		std::array<std::array<std::array<std::array<Player, 3>, 3>, 3>, 3>;

public:
	Game();
	Player playerToMove() const { return playerToMove_; }
	const std::vector<Play>& moves() const;
	void applyMove(const Play& p);
	Player winner() const;
	bool isTerminal() const;
	Play lastPlay() const { return lastPlay_; }

	const GameBoard& getBoard() const { return board; }
	const std::array<std::array<Player, 3>, 3>& getWinCache() const {
		return winCache;
	}
	std::pair<int, int> getNextCoor() const {
		return std::make_pair(nextSubRow, nextSubCol);
	}

	friend std::ostream& operator<<(std::ostream& os, const Game& g);

private:
	Player checkGlobalWin() const;

	void updateWinCache(const Play& p);

	void updateMoveCache();
	std::vector<Play> moveCache; // store moves for current state for reuse
	bool movesGenerated = false;

	Player playerToMove_ = Player::X; // x starts
	int nextSubRow = -1;
	int nextSubCol = -1;
	Play lastPlay_;
	GameBoard board;
	std::array<std::array<Player, 3>, 3> winCache;
};