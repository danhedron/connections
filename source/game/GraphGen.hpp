#pragma once
#ifndef _GRAPHGEN_HPP
#define _GRAPHGEN_HPP
#include <string>
#include <GameTypes.hpp>
#include <functional>

class GameBoard;

typedef std::function<void (const GameBoard&, const std::string& name)> PrepareImageCallback;

class GraphGen
{
	static PrepareImageCallback pic;
public:

	static std::string boardNode(const GameBoard&, const std::string& data = "", const std::string &colour = "red", const std::string id="");

	static std::string graph(const GameBoard&);

	static std::string graphTree(const GameBoard& b, TokenColour first);

	static void setPrepareImageCallback(const PrepareImageCallback& cb)
	{ pic = cb; }
};

#endif
