#include "test_util.hpp"
#include <iostream>
#include <GameBoard.hpp>

std::string colesc(TokenColour t) {
	return t == T_RED ? "\033[91m": "\033[97m";
}

std::string toksym(TokenOrientation o) {
	return o == O_VERTICAL ? "|" : "─";
}

void print_board(const GameBoard& board)
{
	for(size_t r = 0; r < board.getBoardLength(); ++r) {
		std::cout << (board.getRowColour(r) == T_RED ? "  " : "");
		std::string rowesc = colesc(board.getRowColour(r));
		for(size_t c = 0; c < board.getRowSize(r); ++c) {
			std::cout << rowesc << " · ";
			if(board.getColour(r, c) != T_EMPTY) {
				std::cout << colesc(board.getColour(r, c));
				std::cout << toksym(board.getOrientation(r, c));
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << rowesc << " · " << "\033[39m" << std::endl;
	}
}

