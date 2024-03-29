#pragma once
#ifndef _MINMAXAGENT_HPP_
#define _MINMAXAGENT_HPP_
#include "GameTypes.hpp"
#include <GameBoard.hpp>

#include <random>
#include <functional>
#include <map>

class GameBoard;

// Current State, Score, Alpha, Beta, Depth
typedef std::function<void (const GameBoard&, const GameBoard&, float, float, float, size_t, char)> StateEvaluatedCallback;

class Agent
{
private:

	TokenColour playerColour;

	StateEvaluatedCallback secb;

public:

	Agent(TokenColour tc)
		: playerColour(tc) {}

	TokenColour colour() const
	{ return playerColour; }

	TokenColour opponentColour() const
	{ return colour() == T_RED ? T_WHITE : T_RED; }

	virtual Move calculateMove(const GameBoard& board) = 0;

	virtual ~Agent(){}

	void setStateEvaluatedCallback(StateEvaluatedCallback cb)
	{ secb = cb; }

	const StateEvaluatedCallback& stateEvaluatedCallback() const
	{ return secb; }
};

class MinMaxAgent : public Agent
{
	/** Stats */
	unsigned int tally;
	unsigned int cacheHits;

	unsigned int _maxDepth;

	std::random_device rdevice;
	std::default_random_engine rengine;

	float utility(const GameBoard& board);

	float eval(const GameBoard& board);

	float minValue(const GameBoard& board, const Move& move, unsigned int d);

	float maxValue(const GameBoard& board, const Move& move, unsigned int d);

	float value(const GameBoard& board, const GameBoard &parent, bool player, float alpha, float beta, unsigned int d);

	std::map<GameBoard::Hash, float> _scorecache;

public:

	MinMaxAgent(TokenColour player, unsigned int _maxDepth);

	virtual ~MinMaxAgent() {}

	Move calculateMove(const GameBoard& board);

	unsigned int maxDepth() const { return _maxDepth; }

	void setMaxDepth(unsigned int maxDepth) { _maxDepth = maxDepth; }

};

#endif
