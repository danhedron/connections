#include "MinMaxAgent.hpp"
#include "GameBoard.hpp"
#include <limits>

MinMaxAgent::MinMaxAgent(TokenColour player)
	: playerColour(player)
{

}

int MinMaxAgent::utility(const GameBoard& board)
{
	auto w = board.getWinner();
	if(w == playerColour) {
		return 1;
	}
	else if(w == T_EMPTY) {
		return 0;
	}
	return -1;
}

int MinMaxAgent::minValue(const GameBoard& oldboard, const Move& move)
{
    GameBoard board(oldboard);
    board.putToken(move.row, move.column, playerColour == T_RED? T_WHITE : T_RED);
	if(board.isEndGame()) {
		return utility(board);
	}
	auto moves = board.availableMoves();
	int score = std::numeric_limits<int>::max();
	for(auto& move : moves) {
		score = std::min(score, maxValue(board, move));
	}
	return score;
}

int MinMaxAgent::maxValue(const GameBoard& oldboard, const Move& move)
{
    GameBoard board(oldboard);
    board.putToken(move.row, move.column, playerColour);
	if(board.isEndGame()) {
		return utility(board);
	}
	auto moves = board.availableMoves();
	int score = std::numeric_limits<int>::min();
	for(auto& move : moves) {
		score = std::max(score, minValue(board, move));
	}
	return score;
}

Move MinMaxAgent::calculateMove(const GameBoard& board)
{
	auto moves = board.availableMoves();
	Move best;
	int bestScore = std::numeric_limits<int>::min();
	for(auto& move : moves) {
		auto v = maxValue(board, move);
		if(v > bestScore) {
			bestScore = v;
			best = move;
		}
	}
	return best;
}

