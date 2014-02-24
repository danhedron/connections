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
    unsigned int tally;

    float utility(const GameBoard& board);

    float minValue(const GameBoard& board, const Move& move, unsigned int d);

    float maxValue(const GameBoard& board, const Move& move, unsigned int d);

    float value(const GameBoard& board, bool player, unsigned int d);
public:

	MinMaxAgent(TokenColour player);

	Move calculateMove(const GameBoard& board);
};

#endif
