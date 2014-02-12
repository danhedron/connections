#include "GameBoard.hpp"
#include <cassert>

BoardRow::BoardRow(TokenColour c, BoardIndex l)
	: rowColour(c)
{
	tokens.resize(l);
	for(unsigned int i = 0; i < l; ++i) {
		tokens[i] = T_EMPTY;
	}
}

void BoardRow::putToken(BoardIndex i, TokenColour colour)
{
	assert(i < tokens.size());
	tokens[i] = colour;
}

TokenColour BoardRow::getColour(BoardIndex i) const
{
	assert(i < tokens.size());
	return tokens[i];
}

TokenOrientation BoardRow::getOrientation(BoardIndex i) const
{
	assert(i < tokens.size());
	return tokens[i] == rowColour ? O_HORIZONTAL : O_VERTICAL;
}

GameBoard::GameBoard(BoardIndex length)
{
	rows.reserve(length);
	for(unsigned int r = 0; r < length; ++r) {
		rows.push_back(BoardRow(r%2?T_WHITE:T_RED, (r%2!=0)?5:4));
	}
}

void GameBoard::putToken(BoardIndex row, BoardIndex i, TokenColour t)
{
	assert(row < rows.size());
	rows[row].putToken(i, t);
}

TokenColour GameBoard::getColour(BoardIndex row, BoardIndex i) const
{
	assert(row < rows.size());
	return rows[row].getColour(i);
}

TokenOrientation GameBoard::getOrientation(BoardIndex row, BoardIndex i) const
{
	assert(row < rows.size());
	return rows[row].getOrientation(i);
}

BoardIndex GameBoard::getRowSize(BoardIndex row) const
{
	return ((row%2!=0)? 5 : 4);
}

TokenColour GameBoard::getRowColour(BoardIndex row)
{
	return row%2?T_WHITE:T_RED;
}

BoardIndex GameBoard::getBoardLength() const
{
	return rows.size();
}

void GameBoard::reset()
{
	for(unsigned int r = 0; r < rows.size(); ++r) {
		for(unsigned int c = 0; c < ((r%2!=0)?5:4); ++c) {
			rows[r].putToken(c, T_EMPTY);
		}
	}
}

bool GameBoard::isEndGame() const
{
	// Determine if either colour has reached across the board.
	for(unsigned int r = 0; r < rows.size(); ++r) {
		bool broke = false;
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
			if(getColour(r, c) != T_WHITE) {
				broke = true;
				break;
			}
		}
		if(! broke) {
			return true;
		}
	}
	for(unsigned int c = 0; c < rows.size(); ++c) {
		bool broke = false;
		for(unsigned int r = 1; r < rows.size(); r += 2) {
			if(getColour(r, c) != T_RED) {
				broke = true;
				break;
			}
		}
		if(! broke) {
			return true;
		}
	}
	return false;
}

TokenColour GameBoard::getWinner() const
{
	for(unsigned int r = 0; r < rows.size(); ++r) {
		bool broke = false;
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
			if(getColour(r, c) != T_WHITE) {
				broke = true;
				break;
			}
		}
		if(! broke) {
			return T_WHITE;
		}
	}
	for(unsigned int c = 0; c < rows.size(); ++c) {
		bool broke = false;
		for(unsigned int r = 1; r < rows.size(); r += 2) {
			if(getColour(r, c) != T_RED) {
				broke = true;
				break;
			}
		}
		if(! broke) {
			return T_RED;
		}
	}
	return T_EMPTY;
}

