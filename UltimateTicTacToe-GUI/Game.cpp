#include "pch.h"
#include "Game.h"

#if defined(WIN32) || defined(_WIN32) || \
	defined(__WIN32) && !defined(__CYGWIN__)
#define SHOW_COLORS
#include <windows.h>
#endif
using namespace std;

std::string playerToString(const Player& p) {
	switch (p) {
	case Player::X:
		return "X";
	case Player::O:
		return "O";
	case Player::None:
		return "-";
	case Player::Full:
		return "F";
	}
}

ostream& operator<<(ostream& os, const Player& p) {
	os << playerToString(p);
	return os;
}

Game::Game() {
	// initialize all Player values to Player::None
	for (auto& row : board)
		for (auto& col : row)
			for (auto& subRow : col)
				subRow.fill(Player::None);
	for (auto& row : winCache)
		row.fill(Player::None);
}

const vector<Play>& Game::moves() const {
	if (!movesGenerated) {
		const_cast<Game*>(this)->updateMoveCache();
		const_cast<Game*>(this)->movesGenerated = true;
	}
	return moveCache;
}

void Game::applyMove(const Play& p) {
	board[p.row][p.col][p.subRow][p.subCol] = playerToMove_;
	// toggle player to move
	playerToMove_ = playerToMove_ == Player::X ? Player::O : Player::X;
	// update next sub row
	nextSubRow = p.subRow;
	nextSubCol = p.subCol;
	updateWinCache(p); // check for win
	lastPlay_ = p;
	movesGenerated = false; // moves need to be generated again
}

Player Game::winner() const { return checkGlobalWin(); }

bool Game::isTerminal() const {
	return checkGlobalWin() != Player::None || moves().size() == 0;
}

Player Game::checkGlobalWin() const {
	// Checking for Rows for X or O victory.
	for (int row = 0; row < 3; row++) {
		if (winCache[row][0] == winCache[row][1] &&
			winCache[row][1] == winCache[row][2] &&
			winCache[row][0] != Player::None &&
			winCache[row][0] != Player::Full) {
			return winCache[row][0];
		}
	}

	// Checking for Columns for X or O victory.
	for (int col = 0; col < 3; col++) {
		if (winCache[0][col] == winCache[1][col] &&
			winCache[1][col] == winCache[2][col] &&
			winCache[0][col] != Player::None &&
			winCache[0][col] != Player::Full) {
			return winCache[0][col];
		}
	}

	// Checking for Diagonals for X or O victory.
	if (winCache[0][0] == winCache[1][1] && winCache[1][1] == winCache[2][2] &&
		winCache[0][0] != Player::None && winCache[0][0] != Player::Full) {
		return winCache[0][0];
	}

	if (winCache[0][2] == winCache[1][1] && winCache[1][1] == winCache[2][0] &&
		winCache[0][2] != Player::None && winCache[0][2] != Player::Full) {
		return winCache[0][2];
	}

	// Else if none of them have won then return 0
	return Player::None;
	// Win check
}

void Game::updateWinCache(const Play& p) {
	// check for win in sub gameboard
	auto& subBoard = board[p.row][p.col];

	int row = 0, col = 0, diag = 0, adiag = 0; // adiag = anti-diagonal
	Player player =
		playerToMove() == Player::X ? Player::O : Player::X; // previous player
	for (unsigned i = 0; i < 3; i++) {
		if (subBoard[p.subRow][i] == player)
			col++;
		if (subBoard[i][p.subCol] == player)
			row++;
		if (subBoard[i][i] == player)
			diag++;
		if (subBoard[i][2 - i] == player)
			adiag++;
	}
	if (row == 3 || col == 3 || diag == 3 || adiag == 3) {
		winCache[p.row][p.col] = player;
		return;
	}

	// Check for full
	for (auto& row : subBoard) {
		for (auto& col : row) {
			if (col == Player::None) {
				winCache[p.row][p.col] = Player::None;
				return;
			}
		}
	}
	winCache[p.row][p.col] = Player::Full;
}

void Game::updateMoveCache() {
	moveCache.clear();
	// check if subNext == -1 -1 or subWin set
	if ((nextSubRow == -1 && nextSubCol == -1) ||
		winCache[nextSubRow][nextSubCol] != Player::None) {
		moveCache.reserve(81);
		// get all tiles availible
		for (unsigned winRow = 0; winRow < 3; winRow++) {
			for (unsigned winCol = 0; winCol < 3; winCol++) {
				if (winCache[winRow][winCol] == Player::None) {
					// get all empty squares
					for (unsigned subRow = 0; subRow < 3; subRow++) {
						for (unsigned subCol = 0; subCol < 3; subCol++) {
							if (board[winRow][winCol][subRow][subCol] ==
								Player::None) {
								moveCache.emplace_back(
									Play(winRow, winCol, subRow, subCol));
							}
						}
					}
					//
				}
			}
		}
	}

	else {
		if (winCache[nextSubRow][nextSubCol] == Player::None) {
			moveCache.reserve(9);
			// get moves is winCache[nextSubRow][nextSubCol]
			for (unsigned row = 0; row < 3; row++) {
				for (unsigned col = 0; col < 3; col++) {
					if (board[nextSubRow][nextSubCol][row][col] ==
						Player::None) {
						moveCache.emplace_back(
							Play(nextSubRow, nextSubCol, row, col));
					}
				}
			}
		}
	}
	return;
}

ostream& operator<<(ostream& os, const Game& g) {
#ifdef SHOW_COLORS
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	for (int row = 0; row < 3; row++) {
		for (int subRow = 0; subRow < 3; subRow++) {
			for (int col = 0; col < 3; col++) {
#ifdef SHOW_COLORS
				if (row == g.nextSubRow && col == g.nextSubCol)
					SetConsoleTextAttribute(hConsole, 10); // green
				else
					SetConsoleTextAttribute(hConsole, 7); // white
#endif

				for (int subCol = 0; subCol < 3; subCol++) {
					os << " " << g.getBoard()[row][col][subRow][subCol] << " ";
				}
				os << "  ";
			}
			os << '\n';
		}
		os << '\n';
	}
#ifdef SHOW_COLORS
	SetConsoleTextAttribute(hConsole, 7); // white
#endif
	return os;
}