#pragma once
#ifndef _GRAPHGEN_HPP
#define _GRAPHGEN_HPP
#include <string>
#include <GameTypes.hpp>

class GameBoard;

class GraphGen
{
public:

	static std::string boardNode(const GameBoard&, const std::string &colour = "red");

	static std::string graph(const GameBoard&);

	static std::string graphTree(const GameBoard& b, TokenColour first);

};

#endif
