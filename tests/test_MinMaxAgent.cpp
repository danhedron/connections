#include <MinMaxAgent.hpp>
#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"
#include <iostream>

BOOST_AUTO_TEST_SUITE(MinMaxAgentTests)

BOOST_AUTO_TEST_CASE(MinMaxTest)
{
    GameBoard board(2);
    //board.putToken(1,0, T_RED);
    //board.putToken(3,1, T_WHITE);
    //board.putToken(1,1, T_WHITE);
    print_board(board);

    MinMaxAgent redagent(T_RED);
    MinMaxAgent whiteagent(T_WHITE);

    for(int i = 0; ! board.isEndGame(); ++i) {
        if(i%2==0) {
            Move mr = redagent.calculateMove(board);
            board.putToken(mr.row, mr.column, T_RED);
            print_board(board);
        }
        else {
            Move mw = whiteagent.calculateMove(board);
            board.putToken(mw.row, mw.column, T_WHITE);
            print_board(board);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
