#include <GraphGen.hpp>
#include <GameBoard.hpp>
#include <sstream>
#include <MinMaxAgent.hpp>

MinMaxAgent ag(T_WHITE);

std::string GraphGen::boardNode(const GameBoard &b, const std::string& colour)
{
	return b.encodeString() + " [label = \""+ b.toString("", false) +"\", shape=\"box\", color=\""+colour+"\"]";
}

std::string GraphGen::graph(const GameBoard &b)
{
	return "digraph{"
			+ graphTree(b, T_RED)
			+ "}";
}

std::string GraphGen::graphTree(const GameBoard &b, TokenColour first)
{
	std::stringstream ss;
	auto avmoves = b.availableMoves(first);
	ss << boardNode(b, first == T_RED ? "white" : "red") << ";\n";
	for(auto& m : avmoves) {
		auto btmp = b.apply(m, first);
		ss << graphTree(btmp, first == T_RED ? T_WHITE : T_RED);
		ss << b.encodeString() << " -> " << btmp.encodeString() << ";\n";
	}
	return ss.str();
}
