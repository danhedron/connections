#pragma once
#ifndef _GAMETYPES_HPP_
#define _GAMETYPES_HPP_

typedef unsigned char BoardIndex;

struct Move
{
	BoardIndex row;
	BoardIndex column;

	Move(BoardIndex r, BoardIndex c)
		: row(r), column(c) {}

	Move()
		: row(0), column(0) {}

	bool operator==(const Move& r) const {
		return row == r.row && column == r.column;
	}
};

enum TokenOrientation {
	O_VERTICAL,
	O_HORIZONTAL
};
enum TokenColour {
	T_EMPTY = 0,
	T_RED = 1,
	T_WHITE = 2
};

enum WinType {
	WT_NOWIN = 0,
	WT_BOXIN = 1,
	WT_CONNECTION = 2
};

#endif
