#include "MinMaxAgent.hpp"
#include "GameBoard.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>
#include <chrono>

MinMaxAgent::MinMaxAgent(TokenColour player, unsigned int maxDepth)
	: Agent(player), tally(0), cacheHits(0), _maxDepth(maxDepth),
	  rengine(rdevice())
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
	float redscore = 0.f;
	float whitescore = 0.f;
	for(BoardIndex r = 1; r < b.getBoardLength(); r += 2) {
		if(b.getColour(r, 0) != T_WHITE) { continue; }
		std::list<Move> open;
		open.push_back({r, 0});
		std::vector<Move> closed;
		while(!open.empty()) {
			Move& t = open.front();
			closed.push_back(t);
			if(b.getRowColour(t.row) == T_WHITE) {
				whitescore = std::max(whitescore, (float)t.row);
			}
			std::vector<Move> adjacents; adjacents.reserve(6);
			b.getAdjacentPoints(t.row, t.column, adjacents);
			for(Move& m : adjacents) {
				if(b.getColour(m.row, m.column) != T_WHITE) { continue; }
				if(std::find(open.begin(), open.end(), m) != open.end()) { continue; }
				if(std::find(closed.begin(), closed.end(), m) != closed.end()) { continue; }
				open.push_back(m);
			}
			open.pop_front();
		}
	}
	for(BoardIndex c = 0; c < b.getRowSize(1); c += 1) {
		if(b.getColour(1, c) != T_RED) { continue; }
		std::list<Move> open;
		open.push_back({1, c});
		std::vector<Move> closed;
		while(!open.empty()) {
			Move& t = open.front();
			closed.push_back(t);
			if(t.row == b.getBoardLength()-2) {
				whitescore = std::max(whitescore, t.column/2.f);
			}
			std::vector<Move> adjacents; adjacents.reserve(6);
			b.getAdjacentPoints(t.row, t.column, adjacents);
			for(Move& m : adjacents) {
				if(b.getColour(m.row, m.column) != T_RED) { continue; }
				if(std::find(open.begin(), open.end(), m) != open.end()) { continue; }
				if(std::find(closed.begin(), closed.end(), m) != closed.end()) { continue; }
				open.push_back(m);
			}
			open.pop_front();
		}
	}
	return colour() == T_RED ? (redscore - whitescore) : (whitescore - redscore);
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

bool printTally = false;
int tbuff= 0;
int wcachehits = 0;
unsigned int tallyCount = 100000;

#define PRUNE_SYMMETRY 1
#define PRUNE_AB 1

std::chrono::time_point<std::chrono::system_clock> startClock, now;
float MinMaxAgent::value(const GameBoard &board, const GameBoard& parent, bool player, float alpha, float beta, unsigned int d)
{
	tally ++;
	if(tally - tbuff >= tallyCount && printTally) {
		now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - startClock;

		std::cout << tally << " states / " << cacheHits << " cacheH / "
				  << (tally/elapsed.count()) << " states/s / "
				  << maxDepth() << " / "
				  << (100.f*cacheHits)/tally << "% / "
				  << (100.f*wcachehits)/tbuff << "% "
				  << std::endl;

		tbuff = tally;
		wcachehits = 0;
	}

	float statescore = -1.f;

	char scoremode = ' ';

#ifdef PRUNE_SYMMETRY
	auto it = _scorecache.find(board.encodeHash(true));
	if(it != _scorecache.end()) {
		scoremode = 'C';
		statescore = it->second;
		cacheHits++; wcachehits++;
	}
	else
#endif
		if(board.isEndGame()) {
		statescore = utility(board);
		scoremode = 'T';
	}
	else if(d > maxDepth()) {
		statescore = eval(board);
		scoremode = 'E';
	}
	else {
		if(player) {
			for(Move& m : board.availableMoves(colour())) {
				alpha = std::max(alpha, value(board.apply(m, colour()), board, !player, alpha, beta, d+1));
#ifdef PRUNE_AB
				if(beta <= alpha) break;
#endif
			}
			statescore = alpha;
			scoremode = 'A';
		}
		else {
			for(Move& m : board.availableMoves(opponentColour())) {
				beta = std::min(beta, value(board.apply(m, opponentColour()),
											 board, !player, alpha, beta, d+1));
#ifdef PRUNE_AB
				if(beta <= alpha) break;
#endif
			}
			statescore = beta;
			scoremode = 'B';
		}
	}
	if(scoremode != 'C') {
#ifdef PRUNE_SYMMETRY
		_scorecache.insert({board.encodeHash(true), statescore});
#endif
		if(stateEvaluatedCallback()) {
			stateEvaluatedCallback()(board, parent, statescore, alpha, beta, d, scoremode);
		}
	}
	return statescore;
}

Move MinMaxAgent::calculateMove(const GameBoard& board)
{
	tally = 0; tbuff = 0;
	cacheHits = 0;
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

	if(printTally) {
		now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - startClock;
		std::cout << tally << " states / " << cacheHits << " cacheH / "
				  << (100.f*cacheHits)/tally << "% / "
				  << (100.f*wcachehits)/tbuff << "% / "
				  << (tally/elapsed.count()) << " states/s" << std::endl;
		std::cout << "Picked moves with score:  " << bestScore << std::endl;
	}

	// Clear move cache to avoid memory hogging
	_scorecache.clear();

	std::uniform_int_distribution<int> unidist(0, topMoves.size()-1);
	return topMoves.at(unidist(rengine));
}
