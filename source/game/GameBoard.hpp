#pragma once
#ifndef _GAME_BOARD_HPP
#define _GAME_BOARD_HPP
#include <vector>
#include "GameTypes.hpp"

/**
 * Rough notes on grid layout
 * Game board is roughly octagonal
 *
 * 11 Rows of alternating length (4 : 5)
 * Token states := 
 *   Horizontal (Connecting within row)
 *   Vertical (Connecting adjacent rows) 
 * Token Orientation :=
 *   Row Colour => Horizontal
 *   !Row Colour => Vertical
 * Visual Example
 *  N· · · · ·N
 *  · · · · · ·
 *  N· · · · ·N
 *  · · · · · ·
 *  N· · · · ·N
 *  · · · · · ·
 *  N· · · · ·N
 *  · · · · · ·
 *  N· · · · ·N
 *  · · · · · ·
 *  N· · · · ·N
 *  
 */

class BoardRow
{
	std::vector<TokenColour> tokens;
	TokenColour rowColour;
public:
	BoardRow(TokenColour colour, BoardIndex length);

	/**
	 * Put a token into this row
	 */
	void putToken(BoardIndex i, TokenColour colour);

	/**
	 * Returns the token in the row at the given index
	 */
	TokenColour getColour(BoardIndex i) const;

	/**
	 * Returns the orientation in the row at the given index
	 */
	TokenOrientation getOrientation(BoardIndex i) const;
};

class GameBoard
{
	std::vector<BoardRow> rows;
public:
	GameBoard(BoardIndex length);

	void putToken(BoardIndex row, BoardIndex i, TokenColour t);

	TokenColour getColour(BoardIndex row, BoardIndex i) const;

	TokenOrientation getOrientation(BoardIndex row, BoardIndex i) const;

	BoardIndex getRowSize(BoardIndex row) const;

	TokenColour getRowColour(BoardIndex row) const;

	BoardIndex getBoardLength() const;

	std::vector<Move> getAdjacentPoints(BoardIndex r, BoardIndex c) const;

	void makeMove(const Move& m);

	std::vector<Move> availableMoves() const;

	void reset();

	bool isEndGame() const;

	TokenColour getWinner() const;
};

#endif
