#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(GameBoardTests)

BOOST_AUTO_TEST_CASE(Row_Test)
{
	BoardRow row(T_RED, 5);

	// Insert some tokens on this row
	row.putToken(1, T_RED);
	row.putToken(2, T_RED);
	row.putToken(3, T_WHITE);

	BOOST_CHECK_EQUAL( row.getColour(1), T_RED);
	BOOST_CHECK_EQUAL( row.getColour(3), T_WHITE);

	// Validate orientation
	BOOST_CHECK_EQUAL( row.getOrientation(1), O_HORIZONTAL);
	BOOST_CHECK_EQUAL( row.getOrientation(3), O_VERTICAL);
}

BOOST_AUTO_TEST_CASE(Board_Test)
{
	GameBoard board(5);	

	board.putToken(1, 2, T_WHITE);
	board.putToken(2, 2, T_WHITE);

	board.putToken(1, 3, T_RED);
	board.putToken(2, 3, T_RED);

	BOOST_CHECK_EQUAL( board.getColour(1, 2), T_WHITE);
	BOOST_CHECK_EQUAL( board.getColour(2, 2), T_WHITE);
	BOOST_CHECK_EQUAL( board.getColour(1, 3), T_RED);
	BOOST_CHECK_EQUAL( board.getColour(2, 3), T_RED);

	BOOST_CHECK_EQUAL( board.getOrientation(1, 2), O_HORIZONTAL);
	BOOST_CHECK_EQUAL( board.getOrientation(2, 2), O_VERTICAL);

	BOOST_CHECK_EQUAL( board.getOrientation(1, 3), O_VERTICAL);
	BOOST_CHECK_EQUAL( board.getOrientation(2, 3), O_HORIZONTAL);
}

BOOST_AUTO_TEST_SUITE_END()
