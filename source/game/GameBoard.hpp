#pragma once
#ifndef _GAME_BOARD_HPP
#define _GAME_BOARD_HPP
#include <vector>
#include "GameTypes.hpp"
#include <string>
#include <iostream>

/**
  Board hash structure (due to the insane state size..)
*/
template<unsigned int T> struct BoardHashImpl {
	unsigned int data[T] = {};

	bool operator< (const BoardHashImpl<T> &rhs) const {
		for(int i = T-1; i >= 0; --i) {
			if(data[i] < rhs.data[i]) return true;
		}
		return false;
	}

	bool operator== (const BoardHashImpl<T> &rhs) const {
		for(int i = 0; i < T; ++i) {
			if(data[i] != rhs.data[i]) return false;
		}
		return true;
	}
};

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

class GameBoard
{
	std::vector<TokenColour> _tokens;
	BoardIndex _colourLenths[3];
	BoardIndex _length;
	BoardIndex _moves;
public:

	typedef BoardHashImpl<4> Hash;

	GameBoard(BoardIndex length);
	GameBoard(BoardIndex length, const std::string& data);

	void putToken(BoardIndex row, BoardIndex i, TokenColour t);

	TokenColour getColour(BoardIndex row, BoardIndex i) const;

	TokenColour& operator()(BoardIndex r, BoardIndex c);
	const TokenColour& operator()(BoardIndex r, BoardIndex c) const;

	TokenOrientation getOrientation(BoardIndex row, BoardIndex i) const;

	BoardIndex getRunSize() const;

	BoardIndex getRowSize(BoardIndex row) const { return _colourLenths[getRowColour(row)]; }

	bool isTerminalRow(BoardIndex row) const;

	TokenColour getRowColour(BoardIndex row) const;

	BoardIndex getBoardLength() const;

	void getAdjacentPoints(BoardIndex r, BoardIndex c, std::vector<Move>& points) const;

	void getAdjacentPoints(BoardIndex r, BoardIndex c, TokenColour tc, std::vector<Move>& points) const;

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
