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
{
	rows.reserve((length*2)+1);
	for(unsigned int r = 0; r < (length*2)+1; ++r) {
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
	BoardIndex sz = (rows.size()-1)/2;
	return ((row%2!=0)? sz : sz-1);
}

TokenColour GameBoard::getRowColour(BoardIndex row) const
{
	return row%2?T_WHITE:T_RED;
}

BoardIndex GameBoard::getBoardLength() const
{
	return rows.size();
}

std::vector<Move> GameBoard::availableMoves() const
{
	std::vector<Move> moves;
	for(unsigned int r = 0; r < rows.size(); ++r) {
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
	for(unsigned int r = 0; r < rows.size(); ++r) {
		for(unsigned int c = 0; c < getRowSize(r); ++c) {
			rows[r].putToken(c, T_EMPTY);
		}
	}
}

std::vector<Move> GameBoard::getAdjacentPoints(BoardIndex r, BoardIndex c) const
{
	std::vector<Move> points;
	TokenColour tc = getColour(r, c);
	if(getRowColour(r) == T_RED) {
		if( r > 0 ) {
			points.push_back({ r-1, c });
			points.push_back({ r-1, c+1 });
		}
		if( r < rows.size() -1 ) {
			points.push_back({ r+1, c });
			points.push_back({ r+1, c+1 });
		}
		if( tc == T_RED ) {
			if( c > 0 ) {
				points.push_back({ r, c-1 });
			}
			if( c < getRowSize(r)-1 ) {
				points.push_back({ r, c+1 });
			}
		}
		else if( tc == T_WHITE ) {
			if( r > 3 ) {
				points.push_back({ r-2, c });
			}
			if( r < rows.size()-3 ) {
				points.push_back({ r+2, c });
			}
		}
	}
	else {
		if( c > 0 ) {
			points.push_back({ r-1, c-1 });
			points.push_back({ r+1, c-1 });
		}
		if( c < getRowSize(r)-1 ) {
			points.push_back({ r-1, c });
			points.push_back({ r+1, c });
		}
		if(tc == T_RED) {
			if( r > 2 ) {
				points.push_back({ r-2, c });
			}
			if( r < rows.size()-2 ) {
				points.push_back({ r+2, c });
			}
		}
		else if(tc == T_WHITE) {
			if( c > 0 ) {
				points.push_back({ r, c-1 });
			}
			if( c < getRowSize(r)-1 ) {
				points.push_back({ r, c+1 });
			}
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
        std::cout << prefix;
        std::cout << (getRowColour(r) == T_RED ? "  " : "");
        std::string rowesc = colesc(getRowColour(r));
        for(size_t c = 0; c < getRowSize(r); ++c) {
            std::cout << rowesc << " · ";
            if(getColour(r, c) != T_EMPTY) {
                std::cout << colesc(getColour(r, c));
                std::cout << toksym(getOrientation(r, c));
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << rowesc << " · " << "\033[39m" << std::endl;
	}
}

TokenColour GameBoard::getAcrossBoardWinner() const
{
	for(BoardIndex r = 1; r < rows.size(); r += 2) {
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
			if(t.row == rows.size()-2) {
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
		for(BoardIndex c = 0; c < getRowSize(c); ++c) {
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
