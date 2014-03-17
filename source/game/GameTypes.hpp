#pragma once
#ifndef _GAMETYPES_HPP_
#define _GAMETYPES_HPP_

typedef unsigned int BoardIndex;

struct Move
{
	unsigned int row;
	unsigned int column;
	bool operator==(const Move& r) const {
		return row == r.row && column == r.column;
	}
};

enum TokenOrientation {
	O_VERTICAL,
	O_HORIZONTAL
};
enum TokenColour {
	T_EMPTY,
	T_RED,
	T_WHITE
};

enum WinType {
	WT_NOWIN = 0,
	WT_BOXIN = 1,
	WT_CONNECTION = 2
};

#endif
