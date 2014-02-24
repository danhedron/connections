#include "test_util.hpp"
#include <GameBoard.hpp>
#include <iostream>

void print_board(const GameBoard& board)
{
    board.printBoard();

    if(board.isEndGame()) {
        std::string winner = board.getWinner() == T_RED ? "Red" : "White";
        std::cout << "Endgame, winner: " << winner << std::endl;
    }
    else {
        std::cout << "No winner" << std::endl;
    }
}

