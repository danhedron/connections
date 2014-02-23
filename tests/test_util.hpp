#pragma once
#ifndef _TESTUTIL_HPP
#define _TESTUTIL_HPP
#include <string>
#include <GameTypes.hpp>

class GameBoard;

std::string colesc(TokenColour t);

std::string toksym(TokenOrientation o);

void print_board(const GameBoard& board);

#endif
