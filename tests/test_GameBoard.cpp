#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"

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

BOOST_AUTO_TEST_CASE(Board_Test_Size)
{
	GameBoard board(5);

	BOOST_CHECK(board.getBoardLength() == 11);
	BOOST_CHECK(board.getRowSize(0) == 4);
	BOOST_CHECK(board.getRowSize(10) == 4);
	BOOST_CHECK(board.getRowSize(1) == 5);
	BOOST_CHECK(board.getRowSize(2) == 6);
}

BOOST_AUTO_TEST_CASE(Board_Test_Reset)
{
	GameBoard board(5);	

	for(unsigned int i = 0; i < 4; i++) {
		board.putToken(1, i, T_WHITE);
	}

	board.reset();

	for(unsigned int i = 0; i < 4; i++) {
		BOOST_CHECK_EQUAL( board.getColour(1, i), T_EMPTY);
	}
}

BOOST_AUTO_TEST_CASE(Board_Test_GameOver)
{
	GameBoard board(5);

	BOOST_CHECK(! board.isEndGame() );

	for(unsigned int i = 0; i < board.getRowSize(1); i++) {
		board.putToken(1,i, T_WHITE);
	}

	BOOST_CHECK( board.isEndGame() );

	BOOST_CHECK( board.getWinner() == T_WHITE );

	board.reset();

	BOOST_CHECK(! board.isEndGame() );

	for(unsigned int i = 1; i < board.getBoardLength(); i+=2) {
		board.putToken(i, 1, T_RED);
	}

	BOOST_CHECK( board.isEndGame() );

	BOOST_CHECK( board.getWinner() == T_RED );

    GameBoard bn(1);

    BOOST_CHECK(! bn.isEndGame() );

    GameBoard br(1);

    br.putToken(1,0, T_RED);

    BOOST_CHECK( br.isEndGame() );

    BOOST_CHECK( br.getWinner() == T_RED );
}

BOOST_AUTO_TEST_CASE(Board_Test_EndGame_Boxin)
{
	{
		GameBoard board(5);

		board.putToken(0, 0, T_RED);
		board.putToken(0, 1, T_RED);
		board.putToken(0, 2, T_RED);
		board.putToken(1, 0, T_RED);
		board.putToken(1, 3, T_RED);
		board.putToken(2, 0, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 2, T_RED);

		board.putToken(1, 1, T_WHITE);

		board.printBoard();

		BOOST_CHECK( board.isEndGame());
	}

	{
		GameBoard board(5);

		board.putToken(0, 0, T_RED);
		board.putToken(1, 0, T_RED);
		board.putToken(1, 1, T_RED);
		board.putToken(2, 0, T_RED);

		board.printBoard();

		BOOST_CHECK( board.isEndGame());
	}

	{
		GameBoard board(5);
		board.putToken(1, 1, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 0, T_RED);
		board.putToken(3, 1, T_RED);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}

	{
		GameBoard board(5);
		board.putToken(1, 1, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 0, T_RED);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}

	if(false) {
		GameBoard board(5);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 0, T_RED);

		BOOST_CHECK(! board.isEndGame());
	}
}

BOOST_AUTO_TEST_CASE(Board_Test_Adjacent)
{
	GameBoard board(5);

	{
		auto adjacent = board.getAdjacentPoints(0, 0, T_RED);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 1}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 0}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 1}) != adjacent.end());
	}
	{
		auto adjacent = board.getAdjacentPoints(0, 3, T_RED);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 2}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 4}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 3}) != adjacent.end());
	}
	{
		auto adjacent = board.getAdjacentPoints(1, 0, T_WHITE);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 1}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 0}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 1}) != adjacent.end());
	}
	{
		auto adjacent = board.getAdjacentPoints(1, 4, T_WHITE);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 5}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 4}) != adjacent.end());
	}

	for(BoardIndex r = 0; r < board.getBoardLength(); ++r) {
		for(BoardIndex c = 0; c < board.getRowSize(r); ++c) {
			auto adjacents = board.getAdjacentPoints(r, c);
			for(auto& adj : adjacents) {
				BOOST_CHECK_MESSAGE(board.getRowSize(adj.row) > adj.column,
							"RowSize = " << board.getRowSize(adj.row) << " r = " << r << " c = " << c);
			}
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
