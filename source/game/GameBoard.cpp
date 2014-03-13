#include "GameBoard.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>
#include <list>

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
	: _length(length)
{
	_rows.reserve((length*2)+1);
	for(unsigned int r = 0; r < getBoardLength(); ++r) {
		_rows.push_back(BoardRow(getRowColour(r), getRowSize(r)));
	}
}

void GameBoard::putToken(BoardIndex row, BoardIndex i, TokenColour t)
{
	assert(row < _rows.size());
	_rows[row].putToken(i, t);
}

TokenColour GameBoard::getColour(BoardIndex row, BoardIndex i) const
{
	assert(row < _rows.size());
	return _rows[row].getColour(i);
}

TokenOrientation GameBoard::getOrientation(BoardIndex row, BoardIndex i) const
{
	assert(row < _rows.size());
	return _rows[row].getOrientation(i);
}

BoardIndex GameBoard::getRunSize() const
{
	return _length;
}

BoardIndex GameBoard::getRowSize(BoardIndex row) const
{
	auto run = getRunSize();
	return (getRowColour(row)==T_RED? run+1 : run);
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

std::vector<Move> GameBoard::availableMoves() const
{
	std::vector<Move> moves;
	for(unsigned int r = 0; r < _rows.size(); ++r) {
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
            if(getColour(r, c) == T_EMPTY) {
				moves.push_back({r, c});
			}
		}
	}
	return moves;
}

void GameBoard::reset()
{
	for(unsigned int r = 0; r < _rows.size(); ++r) {
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
			_rows[r].putToken(c, T_EMPTY);
		}
	}
}

std::vector<Move> GameBoard::getAdjacentPoints(BoardIndex r, BoardIndex c) const
{
	TokenColour tc = getColour(r, c);
	return getAdjacentPoints(r, c, tc);
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

std::vector<Move> GameBoard::getAdjacentPoints(BoardIndex r, BoardIndex c, TokenColour tc) const
{
	std::vector<Move> points;
	if(getRowColour(r) == T_RED) {
		addIfValid(points, this, r-1, c + 1);
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
	return points;
}

GameBoard GameBoard::apply(const Move &m, TokenColour c) const
{
    GameBoard gb(*this);
    gb.putToken(m.row, m.column, c);
    return gb;
}

bool GameBoard::isEndGame() const
{
    return getWinner() != T_EMPTY;
}

TokenColour GameBoard::getWinner() const
{
	TokenColour ab = getAcrossBoardWinner();
	if(ab != T_EMPTY) return ab;

	TokenColour bi = getBoxInWinner();
	if(bi != T_EMPTY) return bi;

	return T_EMPTY;
}

#include <iostream>

std::string colesc(TokenColour t) {
    return t == T_RED ? "\033[91m": "\033[39m";
}

std::string toksym(TokenOrientation o) {
    return o == O_VERTICAL ? "|" : "─";
}

void GameBoard::printBoard(std::string prefix) const
{
    for(size_t r = 0; r < getBoardLength(); ++r) {
		auto rc = getRowColour(r);
        std::cout << prefix;
		std::cout << (rc == T_RED ? "" : "");
        std::string rowesc = colesc(getRowColour(r));
		if(rc == T_WHITE) std::cout << "· ";
        for(size_t c = 0; c < getRowSize(r); ++c) {
            if(getColour(r, c) != T_EMPTY) {
                std::cout << colesc(getColour(r, c));
                std::cout << toksym(getOrientation(r, c));
            }
            else {
                std::cout << " ";
            }
			if(rc == T_WHITE || c < getRowSize(r)-1) {
				std::cout << rowesc << " · ";
			}
        }
		std::cout << rowesc << "\033[39m" << std::endl;
	}
}

TokenColour GameBoard::getAcrossBoardWinner() const
{
	for(BoardIndex r = 1; r < _rows.size(); r += 2) {
		if(getColour(r, 0) != T_WHITE) { continue; }
		std::list<Move> open;
		open.push_back({r, 0});
		std::vector<Move> closed;
		while(!open.empty()) {
			Move& t = open.front();
			closed.push_back(t);
			if(t.column == getRowSize(r)-1) {
				return T_WHITE;
			}
			auto adjacents = getAdjacentPoints(t.row, t.column);
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
			if(t.row == _rows.size()-2) {
				return T_RED;
			}
			auto adjacents = getAdjacentPoints(t.row, t.column);
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
				auto adj = getAdjacentPoints(r, c);
				for(auto& a : adj) {
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
	bool vertical = lc != rc;
	if(vertical) {
		if(source.row > point.row) {
			if(exit.row >= source.row) return false;
		}
		if(source.row < point.row) {
			if(exit.row <= source.row) return false;
		}
	}
	else {
		if(rc == T_RED) {
			if(source.column <= point.column) {
				if(exit.column <= point.column) return false;
			}
			else if(source.column > point.column) {
				if(exit.column > point.column) return false;
			}
		}
		else {
			if(source.column < point.column) {
				if(exit.column < point.column) return false;
			}
			else if(source.column >= point.column) {
				if(exit.column >= point.column) return false;
			}
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
	auto adj = getAdjacentPoints(point.row, point.column);
	for(auto& a : adj) {
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
