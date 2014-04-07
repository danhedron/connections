#include "MinMaxAgent.hpp"
#include "GameBoard.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>
#include <chrono>

MinMaxAgent::MinMaxAgent(TokenColour player, unsigned int maxDepth)
	: Agent(player), tally(0), cacheHits(0), maxDepth(maxDepth), rengine(rdevice())
{

}

float MinMaxAgent::utility(const GameBoard& board)
{
	auto w = board.getWinner();
	if(w == colour()) {
		return 100.f;
	}
	else if(w == T_EMPTY) {
		return 0.f;
	}
	return -100.f;
}

float MinMaxAgent::eval(const GameBoard &b)
{
	return 0.f; //colour() == T_RED ? -50.f : 50.f;
}

float MinMaxAgent::minValue(const GameBoard& oldboard, const Move& move, unsigned int d)
{
	tally ++;
	GameBoard board(oldboard);
	board.putToken(move.row, move.column, colour());
	if(board.isEndGame()) {
		return utility(board);
	}
	auto moves = board.availableMoves(opponentColour());
	float score = std::numeric_limits<float>::max();
	for(auto& m : moves) {
		score = std::min(score, maxValue(board, m, d+1));
	}
	return score;
}

float MinMaxAgent::maxValue(const GameBoard& oldboard, const Move& move, unsigned int d)
{
	tally ++;
	GameBoard board(oldboard);
	board.putToken(move.row, move.column, opponentColour());
	if(board.isEndGame()) {
		return utility(board);
	}
	auto moves = board.availableMoves(colour());
	float score = -std::numeric_limits<float>::max();
	for(auto& m : moves) {
		score = std::max(score, minValue(board, m, d+1));
	}
	return score;
}

int tbuff= 0;
int wcachehits = 0;
std::chrono::time_point<std::chrono::system_clock> startClock, now;
float MinMaxAgent::value(const GameBoard &board, const GameBoard& parent, bool player, float alpha, float beta, unsigned int d)
{
	tally ++;
	if(tally - tbuff >= 10000) {
		now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - startClock;

		std::cout << tally << " states / " << cacheHits << " cacheH / "
				  << (100.f*cacheHits)/tally << "% / "
				  << (100.f*wcachehits)/tbuff << "% / "
				  << (tally/elapsed.count()) << " states/s" << std::endl;

		tbuff = tally;
		wcachehits = 0;
	}

	float statescore = -1.f;

	char scoremode = ' ';

	auto it = _scorecache.find(board.encodeHash(true));

	if(it != _scorecache.end()) {
		scoremode = 'C';
		statescore = it->second;
		cacheHits++; wcachehits++;
	}
	else if(board.isEndGame()) {
		statescore = utility(board);
		scoremode = 'T';
	}
	else if(d > maxDepth) {
		statescore = eval(board);
		scoremode = 'E';
	}
	else {
		if(player) {
			for(Move& m : board.availableMoves(colour())) {
				alpha = std::max(alpha, value(board.apply(m, colour()), board, !player, alpha, beta, d+1));
				if(beta <= alpha) break;
			}
			statescore = alpha;
			scoremode = 'A';
		}
		else {
			for(Move& m : board.availableMoves(opponentColour())) {
				beta = std::min(beta, value(board.apply(m, opponentColour()),
											 board, !player, alpha, beta, d+1));
				if(beta <= alpha) break;
			}
			statescore = beta;
			scoremode = 'B';
		}
	}
	if(scoremode != 'C') {
		_scorecache.insert({board.encodeHash(true), statescore});
		if(stateEvaluatedCallback()) {
			stateEvaluatedCallback()(board, parent, statescore, alpha, beta, d, scoremode);
		}
	}
	return statescore;
}

Move MinMaxAgent::calculateMove(const GameBoard& board)
{
	tally = 0;
	cacheHits = 0;
	_scorecache.clear();
	startClock = std::chrono::system_clock::now();

	auto moves = board.availableMoves(colour());
	float bestScore = -std::numeric_limits<float>::max();
	std::vector<Move> topMoves;
	for(auto& move : moves) {
		auto v = value(board.apply(move, colour()), board, false,
					   std::numeric_limits<float>::lowest(),
					   std::numeric_limits<float>::max(), 0);
		if(v > bestScore) {
			bestScore = v;
			topMoves.clear();
			topMoves.push_back(move);
		}
		else if(! (v < bestScore) ) {
			topMoves.push_back(move);
		}
	}
	now = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsed = now - startClock;
	std::cout << tally << " states / " << cacheHits << " cacheH / "
			  << (100.f*cacheHits)/tally << "% / "
			  << (100.f*wcachehits)/tbuff << "% / "
			  << (tally/elapsed.count()) << " states/s" << std::endl;
	std::cout << "Picked moves with score:  " << bestScore << std::endl;
	std::uniform_int_distribution<int> unidist(0, topMoves.size()-1);
	return topMoves.at(unidist(rengine));
}
