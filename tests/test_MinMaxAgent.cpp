#include <MinMaxAgent.hpp>
#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"
#include <iostream>

BOOST_AUTO_TEST_SUITE(MinMaxAgentTests)

BOOST_AUTO_TEST_CASE(MinMaxTest)
{
	GameBoard board(5);
	//    board.putToken(1,0, T_RED);
	//    board.putToken(0,0, T_RED);
	//    board.putToken(6,1, T_RED);
	//    board.putToken(5,0, T_WHITE);
	//    board.putToken(0,1, T_WHITE);
	//    board.putToken(2,1, T_WHITE);
	board.putToken(5,0, T_WHITE);
	board.putToken(5,1, T_WHITE);
	print_board(board);

	MinMaxAgent redagent(T_RED, board.getRunSize());
	MinMaxAgent whiteagent(T_WHITE, board.getRunSize());

	for(int i = 0; ! board.isEndGame(); ++i) {
		if(i%2==0) {
			Move mr = redagent.calculateMove(board);
			board.putToken(mr.row, mr.column, T_RED);
			//print_board(board);
		}
		else {
			Move mw = whiteagent.calculateMove(board);
			board.putToken(mw.row, mw.column, T_WHITE);
			// print_board(board);
		}
	}

	print_board(board);
}

BOOST_AUTO_TEST_SUITE_END()
