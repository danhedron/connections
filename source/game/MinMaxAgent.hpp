#pragma once
#ifndef _MINMAXAGENT_HPP_
#define _MINMAXAGENT_HPP_
#include "GameTypes.hpp"

class GameBoard;

class Agent
{
public:
	virtual Move calculateMove(const GameBoard& board) = 0;
};

class MinMaxAgent : public Agent
{
	TokenColour playerColour;

	int utility(const GameBoard& board);

	int minValue(const GameBoard& board, const Move& move);

	int maxValue(const GameBoard& board, const Move& move);
public:

	MinMaxAgent(TokenColour player);

	Move calculateMove(const GameBoard& board);
};

#endif
