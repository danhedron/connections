#include <MinMaxAgent.hpp>
#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"

BOOST_AUTO_TEST_SUITE(MinMaxAgentTests)

BOOST_AUTO_TEST_CASE(MinMaxTest)
{
    GameBoard board(2);
	print_board(board);

    MinMaxAgent redagent(T_RED);
    MinMaxAgent whiteagent(T_WHITE);

    while(! board.isEndGame()) {
        Move mr = redagent.calculateMove(board);
        board.putToken(mr.row, mr.column, T_RED);

        if(board.isEndGame()) {
            break;
        }

        Move mw = whiteagent.calculateMove(board);
        board.putToken(mw.row, mw.column, T_WHITE);

        print_board(board);
    }
}

BOOST_AUTO_TEST_SUITE_END()
