#include <GameBoard.hpp>
#include <boost/test/unit_test.hpp>
#include "test_util.hpp"

BOOST_AUTO_TEST_SUITE(GameBoardTests)

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

BOOST_AUTO_TEST_CASE(Board_Test_ValidMoves)
{
	GameBoard board(5);

	BOOST_CHECK( board.isValidMove(T_RED, 1,0) );
	BOOST_CHECK( board.isValidMove(T_RED, 2,1) );

	BOOST_CHECK(! board.isValidMove(T_RED, 0,0) );
	BOOST_CHECK(! board.isValidMove(T_RED, 2,0) );

	BOOST_CHECK( board.isValidMove(T_WHITE, 1,0) );
	BOOST_CHECK( board.isValidMove(T_WHITE, 2,1) );

	BOOST_CHECK(! board.isValidMove(T_WHITE, 0,0) );
	BOOST_CHECK(! board.isValidMove(T_WHITE, 0,2) );
}

BOOST_AUTO_TEST_CASE(Board_Test_Size)
{
	GameBoard board(5);

	BOOST_CHECK(board.getBoardLength() == 11);
	BOOST_CHECK(board.getRowSize(0) == 6);
	BOOST_CHECK(board.getRowSize(10) == 6);
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

	{
		GameBoard board(3);

		board.putToken(1, 0, T_WHITE);
		board.putToken(1, 1, T_WHITE);
		board.putToken(2, 2, T_WHITE);
		//board.putToken(3, 1, T_WHITE);
		//board.putToken(0, 2, T_RED);

		board.printBoard();

		WinType wt = WT_NOWIN;

		BOOST_CHECK(! board.isEndGame(&wt) );

		BOOST_CHECK_EQUAL( wt, WT_NOWIN );
	}
	{
		GameBoard board(3);

		board.putToken(1, 0, T_WHITE);
		board.putToken(1, 1, T_WHITE);
		board.putToken(1, 2, T_WHITE);
		board.putToken(2, 2, T_WHITE);
		//board.putToken(3, 1, T_WHITE);
		//board.putToken(0, 2, T_RED);

		board.printBoard();

		WinType wt = WT_NOWIN;

		BOOST_CHECK( board.isEndGame(&wt) );

		BOOST_CHECK_EQUAL( wt, WT_CONNECTION );
	}
}

BOOST_AUTO_TEST_CASE(Board_Test_EndGame_Boxin)
{
	{
		GameBoard board(5);

		board.putToken(0, 1, T_RED);
		board.putToken(0, 2, T_RED);
		board.putToken(0, 3, T_RED);
		board.putToken(1, 0, T_RED);
		board.putToken(1, 3, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 2, T_RED);
		board.putToken(2, 3, T_RED);

		board.putToken(1, 1, T_WHITE);

		board.printBoard();

		WinType wt = WT_NOWIN;

		BOOST_CHECK( board.isEndGame(&wt) );

		BOOST_CHECK_EQUAL( wt, WT_BOXIN );
	}

	{
		GameBoard board(5);

		board.putToken(0, 1, T_RED);
		board.putToken(1, 0, T_RED);
		board.putToken(1, 1, T_RED);
		board.putToken(2, 1, T_RED);

		board.printBoard();

		BOOST_CHECK( board.isEndGame());
	}

	{
		GameBoard board(5);
		board.putToken(1, 1, T_RED);
		board.putToken(2, 2, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(3, 1, T_RED);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}

	{
		GameBoard board(5);
		board.putToken(1, 1, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 2, T_RED);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}

	{
		GameBoard board(5);
		board.putToken(3, 1, T_RED);
		board.putToken(4, 2, T_RED);
		board.putToken(3, 2, T_RED);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}
	{
		GameBoard board(5);
		board.putToken(4, 1, T_WHITE);
		board.putToken(5, 1, T_WHITE);
		board.putToken(4, 2, T_WHITE);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}
	{
		GameBoard board(5);
		board.putToken(5, 0, T_WHITE);
		board.putToken(5, 1, T_WHITE);
		board.putToken(5, 2, T_WHITE);
		board.putToken(5, 3, T_WHITE);

		board.printBoard();

		BOOST_CHECK(! board.isEndGame());
	}
	{
		GameBoard board(5);
		board.putToken(1, 0, T_RED);
		board.putToken(3, 0, T_RED);
		board.putToken(5, 0, T_RED);
		board.putToken(6, 1, T_RED);
		board.putToken(6, 2, T_RED);
		board.putToken(7, 0, T_RED);
		board.putToken(8, 1, T_RED);

		board.putToken(7, 1, T_WHITE);
		board.putToken(8, 0, T_WHITE);
		board.putToken(8, 2, T_WHITE);
		board.putToken(10, 0, T_WHITE);
		board.putToken(9, 0, T_WHITE);
		board.putToken(9, 1, T_WHITE);

		board.printBoard();

		WinType wt = WT_NOWIN;

		BOOST_CHECK(! board.isEndGame(&wt) );

		BOOST_CHECK_EQUAL( wt, WT_NOWIN);
	}

	if(false) {
		GameBoard board(5);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 2, T_RED);

		BOOST_CHECK(! board.isEndGame());
	}
}

BOOST_AUTO_TEST_CASE(Board_Test_Adjacent)
{
	GameBoard board(5);

	{
		std::vector<Move> adjacent; adjacent.reserve(6);
		board.getAdjacentPoints(0, 1, T_RED, adjacent);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 2}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 0}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 1}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 0}) == adjacent.end());
	}
	{
		std::vector<Move> adjacent; adjacent.reserve(6);
		board.getAdjacentPoints(0, 4, T_RED, adjacent);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 4}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 5}) == adjacent.end());
	}
	{
		std::vector<Move> adjacent; adjacent.reserve(6);
		board.getAdjacentPoints(1, 0, T_RED, adjacent);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 1}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 1}) != adjacent.end());
	}
	{
		std::vector<Move> adjacent; adjacent.reserve(6);
		board.getAdjacentPoints(1, 0, T_WHITE, adjacent);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 1}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 0}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 1}) != adjacent.end());
	}
	{
		std::vector<Move> adjacent; adjacent.reserve(6);
		board.getAdjacentPoints(1, 4, T_WHITE, adjacent);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{1, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 4}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{0, 5}) == adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 5}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 4}) != adjacent.end());
	}
	{
		std::vector<Move> adjacent; adjacent.reserve(6);
		board.getAdjacentPoints(2, 4, T_RED, adjacent);
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{3, 4}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{3, 3}) != adjacent.end());
		BOOST_CHECK(std::find(std::begin(adjacent), std::end(adjacent), Move{2, 5}) != adjacent.end());
	}

	/*for(BoardIndex r = 0; r < board.getBoardLength(); ++r) {
		for(BoardIndex c = 0; c < board.getRowSize(r); ++c) {
			auto adjacents = board.getAdjacentPoints(r, c);
			for(auto& adj : adjacents) {
				BOOST_CHECK_MESSAGE(board.getRowSize(adj.row) > adj.column,
							"RowSize = " << board.getRowSize(adj.row) << " r = " << r << " c = " << c);
			}
		}
	}*/
}

BOOST_AUTO_TEST_CASE(Board_Test_Path)
{
	{
		GameBoard board(5);

		board.putToken(0, 1, T_RED);
		board.putToken(0, 2, T_RED);
		board.putToken(0, 3, T_RED);
		board.putToken(1, 0, T_RED);
		board.putToken(1, 3, T_RED);
		board.putToken(2, 1, T_RED);
		board.putToken(2, 2, T_RED);
		board.putToken(2, 3, T_RED);

		board.putToken(1, 1, T_WHITE);

		BOOST_CHECK(! board.canFollowPath({8,2}, {7,1}, {8,0}) );

		BOOST_CHECK( board.canFollowPath({7,0}, {8,0}, {9,0}) );
		BOOST_CHECK( board.canFollowPath({7,1}, {8,2}, {9,0}) );

		BOOST_CHECK( board.canFollowPath({8,0}, {8,2}, {9,0}) );

		BOOST_CHECK( board.canFollowPath({1,0}, {0,1}, {0,2}) );
		BOOST_CHECK( board.canFollowPath({0,1}, {0,2}, {0,3}) );
		BOOST_CHECK( board.canFollowPath({0,2}, {0,3}, {1,3}) );
		BOOST_CHECK( board.canFollowPath({0,3}, {1,3}, {2,3}) );
		BOOST_CHECK( board.canFollowPath({1,3}, {2,3}, {2,2}) );
		BOOST_CHECK( board.canFollowPath({2,3}, {2,2}, {2,1}) );


		BOOST_CHECK( board.canFollowPath({0,1}, {0,2}, {1,2}) );
		BOOST_CHECK( board.canFollowPath({0,2}, {1,2}, {2,2}) );
		BOOST_CHECK( board.canFollowPath({1,2}, {2,2}, {2,1}) );
		BOOST_CHECK( board.canFollowPath({2,2}, {2,1}, {1,0}) );

	}
}

std::ostream& operator<<(std::ostream& stream, const GameBoard::Hash& h) {
	stream << h.data[0] << h.data[1] << h.data[2] << h.data[3];
	return stream;
}

BOOST_AUTO_TEST_CASE(Board_Test_copy)
{
	{
		GameBoard b1(5);

		b1.putToken(1, 0, T_RED);
		b1.putToken(1, 1, T_RED);

		{
			GameBoard b2(b1);
		}
	}
}
BOOST_AUTO_TEST_CASE(Board_Hash_test)
{
	{
		GameBoard b1(5);
		GameBoard b2(5);

		b1.putToken(1, 0, T_RED);
		b2.putToken(1, 1, T_RED);

		GameBoard::Hash h1 = b1.encodeHash(true);
		GameBoard::Hash h2 = b2.encodeHash(true);

		std::cout << h1 << " " << h2 <<  std::endl;

		BOOST_CHECK( (h1 == h2) == false );
	}
}

BOOST_AUTO_TEST_CASE(Board_Symmetry_test)
{
	{
		GameBoard b1(5);
		GameBoard b2(5);

		b1.putToken(1, 0, T_RED);
		b2.putToken(1, 4, T_RED);

		BOOST_CHECK( b1 < b2 );
	}
	{
		GameBoard b1(5);
		GameBoard b2(5);

		b1.putToken(1, 0, T_RED);
		b2.putToken(9, 0, T_RED);

		BOOST_CHECK( b1 < b2 );
	}
	{
		GameBoard b1(5);
		GameBoard b2(5);

		b1.putToken(1, 0, T_RED);
		b2.putToken(1, 4, T_RED);

		GameBoard::Hash h1 = b1.encodeHash(true);
		GameBoard::Hash h2 = b2.encodeHash(true);
		GameBoard::Hash h3 = b2.encodeHash(false);

		BOOST_CHECK(h1 == h2);
		BOOST_CHECK((h1 < h2) == false);
		BOOST_CHECK((h1 == h3) == false);
	}
	{
		GameBoard b1(5);
		GameBoard b2(5);

		b1.putToken(1, 0, T_RED);
		b2.putToken(9, 0, T_RED);

		GameBoard::Hash h1 = b1.encodeHash(true);
		GameBoard::Hash h2 = b2.encodeHash(true);

		BOOST_CHECK(h1 == h2);
		BOOST_CHECK((h1 < h2) == false);
	}
}

BOOST_AUTO_TEST_SUITE_END()
