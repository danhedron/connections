#pragma once
#ifndef _GAME_BOARD_HPP
#define _GAME_BOARD_HPP

/**
 * Rough notes on grid layout
 * Game board is roughly octagonal
 *
 * 11 Rows of alternating length (6 : 5)
 * Token states := 
 *   Horizontal (Connecting within row)
 *   Vertical (Connecting adjacent rows) 
 * Token Orientation :=
 *   Row Colour => Horizontal
 *   !Row Colour => Vertical
 */

enum TokenOrientation {
	O_VERTICAL,
	O_HORIZONTAL
};
enum TokenColour {
	T_EMPTY,
	T_RED,
	T_WHITE
};

class BoardRow
{
	std::vector<ToeknColour> tokens;
	TokenColour rowColour;
public:
	BoardRow(TokenColour colour, size_t length);

	/**
	 * Put a token into this row
	 */
	void putToken(size_t i, TokenColour colour);

	/**
	 * Returns the token in the row at the given index
	 */
	TokenColour getColour(size_t i) const;

	/**
	 * Returns the orientation in the row at the given index
	 */
	TokenOrientation getOrientation(size_t i) const;
};

class GameBoard
{
public:
	GameBoard(size_t length);
};

#endif
