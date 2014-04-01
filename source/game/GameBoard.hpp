#pragma once
#ifndef _GAME_BOARD_HPP
#define _GAME_BOARD_HPP
#include <vector>
#include "GameTypes.hpp"
#include <string>

/**
 * Rough notes on grid layout
 *
 * Token states :=
 *   Horizontal (Connecting within row)
 *   Vertical (Connecting adjacent rows)
 * Token Orientation :=
 *   Row Colour => Horizontal
 *   !Row Colour => Vertical
 *
 * Visual Example
 *  N· · · · ·N
 *  · · · · · ·
 *  |· · · · ·|
 *  · · · · · ·
 *  |· · · · ·|
 *  · · · · · ·
 *  |· · · · ·|
 *  · · · · · ·
 *  |· · · · ·|
 *  · · · · · ·
 *  N· · · · ·N
 *
 *  Alternate Row Lengths
 *   N, N + 1
 *  Terminal Row Length
 *   N - 1
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

	/**
	 * Returns the colour of the token at the given index.
	 */
	TokenColour operator[](BoardIndex i) const {
		return tokens[i];
	}
};

class GameBoard
{
	std::vector<BoardRow> _rows;
	BoardIndex _colourLenths[3];
	BoardIndex _length;
public:
	GameBoard(BoardIndex length);
	GameBoard(BoardIndex length, const std::string& data);

	void putToken(BoardIndex row, BoardIndex i, TokenColour t);

	TokenColour getColour(BoardIndex row, BoardIndex i) const;

	const BoardRow& operator[](BoardIndex r) const { return _rows[r]; }

	TokenOrientation getOrientation(BoardIndex row, BoardIndex i) const;

	BoardIndex getRunSize() const;

	BoardIndex getRowSize(BoardIndex row) const { return _colourLenths[getRowColour(row)]; }

	bool isTerminalRow(BoardIndex row) const;

	TokenColour getRowColour(BoardIndex row) const;

	BoardIndex getBoardLength() const;

	std::vector<Move> getAdjacentPoints(BoardIndex r, BoardIndex c) const;

	std::vector<Move> getAdjacentPoints(BoardIndex r, BoardIndex c, TokenColour tc) const;

	void makeMove(const Move& m);

	std::vector<Move> availableMoves(TokenColour tc) const;

	bool isValidMove(TokenColour colour, BoardIndex r, BoardIndex c) const;

	GameBoard apply(const Move& m, TokenColour c) const;

	void reset();

	bool isEndGame(WinType* wt = nullptr) const;

	TokenColour getWinner(WinType *wt = nullptr) const;

	/** Prints board to stdout */
	void printBoard(std::string prefix="") const;

	bool operator< (const GameBoard& r) const;

	/**
	 * Returns all symmetries of the current board (not including the current)
	*/
	std::vector<GameBoard> getSymmetries() const;

	std::string toString(const std::string& prefix = "", bool colour = true) const;

	typedef std::string Hash;
	Hash encodeHash(bool normalize = false) const;

	std::string encodeString() const;

private:
	TokenColour getAcrossBoardWinner() const;
	TokenColour getBoxInWinner() const;
public:
	bool canFollowPath(const Move& source, const Move& point, const Move& exit) const;
	bool hasReturnPath(const Move& point, const Move& source, const Move& target, std::vector<Move> history) const;
};

#endif
