#include "MinMaxAgent.hpp"
#include "GameBoard.hpp"
#include <limits>
#include <string>
#include <iostream>

MinMaxAgent::MinMaxAgent(TokenColour player)
    : playerColour(player), tally(0)
{

}

float MinMaxAgent::utility(const GameBoard& board)
{
	auto w = board.getWinner();
	if(w == playerColour) {
        return 1.f;
	}
	else if(w == T_EMPTY) {
        return 0.f;
	}
    return -1.f;
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
	Move best;
    float bestScore = -std::numeric_limits<float>::max();
    for(auto& move : moves) {
        auto v = value(board.apply(move, playerColour), false,
                       std::numeric_limits<float>::lowest(),
                       std::numeric_limits<float>::max(), 1);
        if(v > bestScore) {
            bestScore = v;
			best = move;
        }
	}
    std::cout << "Evaulated " << tally << " states" << std::endl;
	return best;
}
