#pragma once

#include <vector>

template <typename Play, typename Player>
class IGameState {
public:
	virtual Player playerToMove() const = 0;
	virtual const std::vector<Play>& moves() const = 0;
	virtual void applyMove(const Play& p) = 0;
	virtual Player winner() const = 0;
	virtual bool isTerminal() const = 0;
	virtual Play lastPlay() const = 0;
};