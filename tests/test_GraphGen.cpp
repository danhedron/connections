#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"
#include <GraphGen.hpp>

BOOST_AUTO_TEST_SUITE(GraphGenTests)

BOOST_AUTO_TEST_CASE(GenNode)
{
	GameBoard board(5);

	board.putToken(0, 1, T_RED);
	board.putToken(0, 2, T_RED);
	board.putToken(0, 3, T_RED);
	board.putToken(1, 0, T_RED);
	board.putToken(1, 3, T_RED);
	board.putToken(2, 1, T_RED);
	board.putToken(2, 2, T_RED);
	board.putToken(2, 3, T_WHITE);

	//BOOST_CHECK_EQUAL(GraphGen::boardNode(board), "_RRR__R__R__RRW______________________________________________");
}

BOOST_AUTO_TEST_CASE(GenChildren)
{
	GameBoard board(5);

	board.putToken(0, 1, T_RED);
	board.putToken(0, 2, T_RED);
	board.putToken(0, 3, T_RED);
	board.putToken(1, 0, T_RED);
	board.putToken(1, 3, T_RED);
	board.putToken(2, 1, T_RED);
	board.putToken(2, 2, T_RED);
	board.putToken(2, 3, T_WHITE);

	/*BOOST_CHECK_EQUAL(GraphGen::graph(board, 2),
					  "graph{"
					  "_RRR__R__R__RRW______________________________________________"
					  "}");*/
}

BOOST_AUTO_TEST_SUITE_END()
