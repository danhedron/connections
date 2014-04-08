#include "GameBoard.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <list>
#include <sstream>

GameBoard::GameBoard(BoardIndex length)
	: _length(length)
{
	_colourLenths[T_EMPTY] = 0;
	_colourLenths[T_RED] = _length+1;
	_colourLenths[T_WHITE] = _length;

	size_t redRows = length+1;
	size_t whiteRows = length;

	_tokens.resize((redRows * getRowSize(0)) + (whiteRows * getRowSize(1)), T_EMPTY);
}

GameBoard::GameBoard(BoardIndex length, const std::string &data)
	: GameBoard(length)
{
	size_t i = 0;
	for(size_t r = 0; r < getBoardLength(); ++r) {
		for(size_t c = 0; c < getRowSize(r); ++c) {
			char t = data.at(i++);
			if(t != '_') {
				putToken(r, c, t == 'R' ? T_RED : T_WHITE);
			}
		}
	}
}

void GameBoard::putToken(BoardIndex row, BoardIndex i, TokenColour t)
{
	assert(row < getBoardLength());
	_tokens[calculateIndex(row, i)] = t;
}

TokenColour GameBoard::getColour(BoardIndex row, BoardIndex i) const
{
	assert(row < getBoardLength());
	return _tokens[calculateIndex(row, i)];
}

TokenColour &GameBoard::operator()(BoardIndex r, BoardIndex c)
{
	return _tokens[calculateIndex(r, c)];
}

const TokenColour &GameBoard::operator()(BoardIndex r, BoardIndex c) const
{
	return _tokens[calculateIndex(r, c)];
}

TokenOrientation GameBoard::getOrientation(BoardIndex row, BoardIndex i) const
{
	assert(row < getBoardLength());
	return getColour(row, i) == getRowColour(row) ? O_HORIZONTAL : O_VERTICAL;
}

BoardIndex GameBoard::getRunSize() const
{
	return _length;
}

bool GameBoard::isTerminalRow(BoardIndex row) const
{
	if(row == 0) return true;
	if(getBoardLength()-1 == row) return true;
	return false;
}

TokenColour GameBoard::getRowColour(BoardIndex row) const
{
	return row%2?T_WHITE:T_RED;
}

BoardIndex GameBoard::getBoardLength() const
{
	return (_length*2)+1;
}

std::vector<Move> GameBoard::availableMoves(TokenColour tc) const
{
	std::vector<Move> moves;
	for(unsigned int r = 0; r < getBoardLength(); ++r) {
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
			if(getColour(r, c) == T_EMPTY && isValidMove(tc, r, c)) {
				moves.push_back({r, c});
			}
		}
	}
	return moves;
}

bool GameBoard::isValidMove(TokenColour colour, BoardIndex r, BoardIndex c) const
{
	if((r == 0 || r == getBoardLength()-1) && (c == 0 || c == getRowSize(r)-1)) return false;
	if(colour == T_WHITE) {
		if(r == 0 || r == getBoardLength()-1) return false;
	}
	if(colour == T_RED) {
		if(getRowColour(r) == T_RED && (c == 0 || c == getRowSize(r)-1)) return false;
	}
	return true;
}

void GameBoard::reset()
{
	for(unsigned int r = 0; r < getBoardLength(); ++r) {
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
			(*this)(r,c) = T_EMPTY;
		}
	}
}

void GameBoard::getAdjacentPoints(BoardIndex r, BoardIndex c, std::vector<Move>& points) const
{
	TokenColour tc = getColour(r, c);
	getAdjacentPoints(r, c, tc, points);
}

void addIfValid(std::vector<Move>& p, const GameBoard* b, BoardIndex r, BoardIndex c)
{
	if(r >= b->getBoardLength()) return;
	if(c >= b->getRowSize(r)) return;
	if(r == 0 || r == b->getBoardLength()-1) {
		if(c == 0) return;
		if(c == b->getRowSize(r)-1) return;
	}
	p.push_back({r,c});
}

void GameBoard::getAdjacentPoints(BoardIndex r, BoardIndex c, TokenColour tc, std::vector<Move>& points) const
{
	if(getRowColour(r) == T_RED) {
		addIfValid(points, this, r-1, c - 1);
		addIfValid(points, this, r-1, c    );

		addIfValid(points, this, r+1, c - 1);
		addIfValid(points, this, r+1, c    );

		if( tc == T_RED ) {
			addIfValid(points, this, r, c-1);
			addIfValid(points, this, r, c+1);
		}
		else if( tc == T_WHITE ) {
			addIfValid(points, this, r-2, c);
			addIfValid(points, this, r+2, c);
		}
	}
	else {
		addIfValid(points, this, r-1, c  );
		addIfValid(points, this, r-1, c+1);

		addIfValid(points, this, r+1, c  );
		addIfValid(points, this, r+1, c+1);

		if(tc == T_RED) {
			addIfValid(points, this, r-2, c);
			addIfValid(points, this, r+2, c);
		}
		else if(tc == T_WHITE) {
			addIfValid(points, this, r, c-1);
			addIfValid(points, this, r, c+1);
		}
	}
}

GameBoard GameBoard::apply(const Move &m, TokenColour c) const
{
	GameBoard gb(getRunSize(), encodeString());
	gb.putToken(m.row, m.column, c);
	return gb;
}

bool GameBoard::isEndGame(WinType *wt) const
{
	return getWinner(wt) != T_EMPTY;
}

TokenColour GameBoard::getWinner(WinType *wt) const
{
	TokenColour ab = getAcrossBoardWinner();
	if(ab != T_EMPTY) {
		if(wt != nullptr) *wt = WT_CONNECTION;
		return ab;
	}

	TokenColour bi = getBoxInWinner();
	if(bi != T_EMPTY) {
		if(wt != nullptr) *wt = WT_BOXIN;
		return bi;
	}

	if(wt != nullptr) *wt = WT_NOWIN;
	return T_EMPTY;
}

std::string colesc(TokenColour t)
{
	return t == T_RED ? "\033[91m": "\033[39m";
}

std::string toksym(TokenOrientation o)
{
	return o == O_VERTICAL ? "|" : "─";
}

void GameBoard::printBoard(std::string prefix) const
{
	std::cout << toString(prefix) << std::endl;
}

bool GameBoard::operator<(const GameBoard &rhs) const
{
	for(int r = getBoardLength()-1; r >= 0; --r) {
		for(int c = getRowSize(r) - 1; c >= 0; --c) {
			auto tL = (*this)(r,c);
			auto tR = rhs(r,c);
			if(tL == T_EMPTY && tR == T_EMPTY) continue;
			return tL < tR;
		}
	}
	return false;
}

std::vector<GameBoard> GameBoard::getSymmetries() const
{
	std::vector<GameBoard> syms;
	syms.reserve(5);

	// Swap H
	{
		GameBoard b(getRunSize());
		for(size_t r = 0; r < getBoardLength(); ++r) {
			for(size_t c = 0; c < getRowSize(r); ++c) {
				auto T = getColour(r, c);
				if(T != T_EMPTY) {
					b.putToken(r, getRowSize(r) - 1 - c, T);
				}
			}
		}
		syms.push_back(b);
	}
	// Swap V
	{
		GameBoard b(getRunSize());
		for(size_t r = 0; r < getBoardLength(); ++r) {
			for(size_t c = 0; c < getRowSize(r); ++c) {
				auto T = getColour(r, c);
				if(T != T_EMPTY) {
					b.putToken(getBoardLength() - 1 - r, c, T);
				}
			}
		}
		syms.push_back(b);
	}

	// Swap VH
	{
		GameBoard b(getRunSize());
		for(size_t r = 0; r < getBoardLength(); ++r) {
			for(size_t c = 0; c < getRowSize(r); ++c) {
				auto T = getColour(r, c);
				if(T != T_EMPTY) {
					b.putToken(getBoardLength() - 1 - r,
							   getRowSize(r) - 1 - c, T);
				}
			}
		}
		syms.push_back(b);
	}

	return syms;
}

std::string GameBoard::toString(const std::string &prefix, bool colour) const
{
	std::stringstream ss;
	for(size_t r = 0; r < getBoardLength(); ++r) {
		auto rc = getRowColour(r);
		ss << prefix;
		ss << (rc == T_RED ? "" : "");
		std::string rowesc = colour ? colesc(getRowColour(r)) : "";
		if(rc == T_WHITE) ss << "· ";
		for(size_t c = 0; c < getRowSize(r); ++c) {
			if(getColour(r, c) != T_EMPTY) {
				if(colour) ss << colesc(getColour(r, c));
				ss << toksym(getOrientation(r, c));
			}
			else {
				ss << " ";
			}
			if(rc == T_WHITE || c < getRowSize(r)-1) {
				ss << rowesc << " · ";
			}
		}
		ss << rowesc << (colour ? "\033[39m" : "") << (colour ? "\n" : "\\n");
	}
	return ss.str();
}

GameBoard::Hash GameBoard::encodeHash(bool normalize) const
{
	if(! normalize) {
		Hash h;

		int i = 0;
		static int magic = sizeof(unsigned int)*8;
		for(BoardIndex r = 0; r < getBoardLength(); ++r) {
			for(BoardIndex c = 0; c < getRowSize(r); ++c) {
				int byte = (i/magic);
				if((*this)(r,c) == T_RED) {
					h.data[byte] |= (1 << (i%magic));
				} else if((*this)(r,c) == T_WHITE) {
					h.data[byte] |= (2 << (i%magic));
				}
				i+=2;
			}
		}

		return h;
	}

	// find the board symmetry with the lowest value, and return that.
	GameBoard b = *this;
	for(auto sb : getSymmetries()) {
		if(sb < b) b = sb;
	}

	return b.encodeHash(false);
}

std::string GameBoard::encodeString() const
{
	std::stringstream ss;
	for(size_t r = 0; r < getBoardLength(); ++r) {
		for(size_t c = 0; c < getRowSize(r); ++c) {
			if(getColour(r, c) == T_WHITE) {
				ss << "W";
			}
			else if(getColour(r,c) == T_RED) {
				ss << "R";
			}
			else {
				ss << "_";
			}
		}
	}
	return ss.str();
}

TokenColour GameBoard::getAcrossBoardWinner() const
{
	for(BoardIndex r = 1; r < getBoardLength(); r += 2) {
		if(getColour(r, 0) != T_WHITE) { continue; }
		std::list<Move> open;
		open.push_back({r, 0});
		std::vector<Move> closed;
		while(!open.empty()) {
			Move& t = open.front();
			closed.push_back(t);
			if(getRowColour(t.row) == T_WHITE && t.column == getRowSize(t.row)-1) {
				return T_WHITE;
			}
			std::vector<Move> adjacents(8);
			getAdjacentPoints(t.row, t.column, adjacents);
			for(Move& m : adjacents) {
				if(getColour(m.row, m.column) != T_WHITE) { continue; }
				if(std::find(open.begin(), open.end(), m) != open.end()) { continue; }
				if(std::find(closed.begin(), closed.end(), m) != closed.end()) { continue; }
				open.push_back(m);
			}
			open.pop_front();
		}
	}
	for(BoardIndex c = 0; c < getRowSize(1); c += 1) {
		if(getColour(1, c) != T_RED) { continue; }
		std::list<Move> open;
		open.push_back({1, c});
		std::vector<Move> closed;
		while(!open.empty()) {
			Move& t = open.front();
			closed.push_back(t);
			if(t.row == getBoardLength()-2) {
				return T_RED;
			}
			std::vector<Move> adjacents(8);
			getAdjacentPoints(t.row, t.column, adjacents);
			for(Move& m : adjacents) {
				if(getColour(m.row, m.column) != T_RED) { continue; }
				if(std::find(open.begin(), open.end(), m) != open.end()) { continue; }
				if(std::find(closed.begin(), closed.end(), m) != closed.end()) { continue; }
				open.push_back(m);
			}
			open.pop_front();
		}
	}
	return T_EMPTY;
}

TokenColour GameBoard::getBoxInWinner() const
{
	/* foreach t in board
	 *  if has return path()
	 *   return t's colour.
	 *
	 * has return path(point, source, target)
	 *  if point == target return true
	 *  foreach adjacent(point) - source
	 *   if return path(adj, point, target) return true
	 *  return false
	 */

	for(BoardIndex r = 0; r < getBoardLength(); ++r) {
		for(BoardIndex c = 0; c < getRowSize(r); ++c) {
			TokenColour lc = getColour(r ,c);
			if(lc != T_EMPTY) {
				std::vector<Move> adjacents(8);
				getAdjacentPoints(r, c, adjacents);
				for(auto& a : adjacents) {
					if(getColour(a.row, a.column) != lc) continue;
					if (hasReturnPath(a, {r,c}, {r,c}, {})) {
						return lc;
					}
				}
			}
		}
	}

	return T_EMPTY;
}

bool GameBoard::canFollowPath(const Move &source, const Move &point, const Move &exit) const
{
	if(source == exit) return false;
	TokenColour lc = getColour(point.row, point.column);
	TokenColour rc = getRowColour(point.row);

	Move pSource = {source.row, source.column * 2 + (getRowColour(source.row)==T_WHITE?1:0)};
	Move pPoint = {point.row,   point.column * 2  + (getRowColour(point.row)==T_WHITE?1:0)};
	Move pExit = {exit.row,     exit.column * 2   + (getRowColour(exit.row)==T_WHITE?1:0)};

	bool vertical = lc != rc;
	if(vertical) {
		if(pSource.row > pPoint.row) {
			if(pExit.row >= pSource.row) return false;
		}
		if(pSource.row < pPoint.row) {
			if(pExit.row <= pSource.row) return false;
		}
	}
	else {
		if(pSource.column < pPoint.column) {
			if(pExit.column < pPoint.column) return false;
		}
		if(pSource.column > pPoint.column) {
			if(pExit.column > pPoint.column) return false;
		}
	}
	return true;
}

bool GameBoard::hasReturnPath(const Move &point, const Move &source, const Move &target, std::vector<Move> history) const
{
	if(point == target) return true;
	if(std::find(history.begin(), history.end(), point)  != history.end()) return true;
	history.push_back(source);
	auto lc = getColour(point.row, point.column);
	std::vector<Move> adjacents(8);
	getAdjacentPoints(point.row, point.column, adjacents);
	for(auto& a : adjacents) {
		if(getColour(a.row, a.column) != lc) continue;
		if(! canFollowPath(source, point, a)) {
			continue;
		}
		if(hasReturnPath(a, point, target, history)) {
			return true;
		}
	}
	return false;
}
