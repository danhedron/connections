#include "MinMaxAgent.hpp"
#include "GameBoard.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <list>
#include <algorithm>

MinMaxAgent::MinMaxAgent(TokenColour player)
	: Agent(player), tally(0), rengine(rdevice())
{

}

float MinMaxAgent::utility(const GameBoard& board)
{
	auto w = board.getWinner();
	if(w == playerColour) {
        return 100.f;
	}
	else if(w == T_EMPTY) {
        return 0.f;
	}
    return -100.f;
}

float MinMaxAgent::eval(const GameBoard &b)
{
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
                whitescore = std::max(whitescore, (float)t.column);
            }
            auto adjacents = b.getAdjacentPoints(t.row, t.column);
            for(Move& m : adjacents) {
                if(b.getColour(m.row, m.column) != T_WHITE) { continue; }
                if(std::find(open.begin(), open.end(), m) != open.end()) { continue; }
                if(std::find(closed.begin(), closed.end(), m) != closed.end()) { continue; }
                open.push_back(m);
            }
            open.pop_front();
        }
    }
    float redscore = 0.f;
    for(BoardIndex c = 0; c < b.getRowSize(1); c += 1) {
        if(b.getColour(1, c) != T_RED) { continue; }
        std::list<Move> open;
        open.push_back({1, c});
        std::vector<Move> closed;
        while(!open.empty()) {
            Move& t = open.front();
            closed.push_back(t);
            if(b.getRowColour(t.row) == T_WHITE) {
                redscore = std::max(redscore, (float)t.row);
            }
            auto adjacents = b.getAdjacentPoints(t.row, t.column);
            for(Move& m : adjacents) {
                if(b.getColour(m.row, m.column) != T_RED) { continue; }
                if(std::find(open.begin(), open.end(), m) != open.end()) { continue; }
                if(std::find(closed.begin(), closed.end(), m) != closed.end()) { continue; }
                open.push_back(m);
            }
            open.pop_front();
        }
    }
    return playerColour == T_RED ? redscore-whitescore : whitescore-redscore;
}

float MinMaxAgent::minValue(const GameBoard& oldboard, const Move& move, unsigned int d)
{
    tally ++;
    GameBoard board(oldboard);
    board.putToken(move.row, move.column, (playerColour == T_RED)? T_WHITE : T_RED);
    if(board.isEndGame()) {
        return utility(board);
    }
    auto moves = board.availableMoves();
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
    board.putToken(move.row, move.column, playerColour);
    if(board.isEndGame()) {
       return utility(board);
    }
    auto moves = board.availableMoves();
    float score = -std::numeric_limits<float>::max();
    for(auto& m : moves) {
        score = std::max(score, minValue(board, m, d+1));
	}
    return score;
}

int tbuff= 0;
float MinMaxAgent::value(const GameBoard &board, bool player, float al, float bet, unsigned int d)
{
    tally ++;
    if(tally - tbuff >= 10000) {
        std::cout << tally << " states" << std::endl;
        tbuff = tally;
    }

    if(d > (board.getBoardLength()-1)/2) {
        return eval(board);
    }
    if(board.isEndGame()) {
        return utility(board);
    }

    if(player) {
        for(Move& m : board.availableMoves()) {
            al = std::max(al, value(board.apply(m, playerColour), al, bet, !player, d+1));
            if(al >= bet) return bet;
        }
        return al;
    }
    else {
        for(Move& m : board.availableMoves()) {
            bet = std::min(bet, value(board.apply(m, (playerColour==T_RED)? T_WHITE:T_RED),
                                         al, bet, !player, d+1));
            if(bet <= al) return al;
        }
        return bet;
    }
}

Move MinMaxAgent::calculateMove(const GameBoard& board)
{
	tally = 0;
	auto moves = board.availableMoves();
	float bestScore = -std::numeric_limits<float>::max();
	std::vector<Move> topMoves;
	for(auto& move : moves) {
		auto v = value(board.apply(move, playerColour), false,
					   std::numeric_limits<float>::lowest(),
					   std::numeric_limits<float>::max(), 1);
		if(v > bestScore) {
			bestScore = v;
			topMoves.clear();
			topMoves.push_back(move);
		}
		else if(! (v < bestScore) ) {
			topMoves.push_back(move);
		}
	}
	std::cout << "Evaulated " << tally << " states" << std::endl;
	std::uniform_int_distribution<int> unidist(0, topMoves.size()-1);
	return topMoves.at(unidist(rengine));
}
