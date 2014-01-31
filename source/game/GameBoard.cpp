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
		rows.push_back(BoardRow(r%2?T_WHITE:T_RED, r%2?5:4));
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

