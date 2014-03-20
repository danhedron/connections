#pragma once
#ifndef _MINMAXAGENT_HPP_
#define _MINMAXAGENT_HPP_
#include "GameTypes.hpp"
#include <random>

class GameBoard;

class Agent
{
private:

	TokenColour playerColour;

public:

	Agent(TokenColour tc)
		: playerColour(tc) {}

	TokenColour colour() const
	{ return playerColour; }

	TokenColour opponentColour() const
	{ return colour() == T_RED ? T_WHITE : T_RED; }

	virtual Move calculateMove(const GameBoard& board) = 0;

	virtual ~Agent(){}
};

class MinMaxAgent : public Agent
{
	unsigned int tally;

	std::random_device rdevice;
	std::default_random_engine rengine;

	float utility(const GameBoard& board);

	float eval(const GameBoard& board);

	float minValue(const GameBoard& board, const Move& move, unsigned int d);

	float maxValue(const GameBoard& board, const Move& move, unsigned int d);

	float value(const GameBoard& board, bool player, float alpha, float beta, unsigned int d);
public:

	MinMaxAgent(TokenColour player);

	Move calculateMove(const GameBoard& board);
};

#endif
