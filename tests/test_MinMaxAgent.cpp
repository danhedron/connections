#include <MinMaxAgent.hpp>
#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"

BOOST_AUTO_TEST_SUITE(MinMaxAgentTests)

BOOST_AUTO_TEST_CASE(MinMaxTest)
{
	GameBoard board(5);
	print_board(board);
}

BOOST_AUTO_TEST_SUITE_END()
