#include <GraphGen.hpp>
#include <GameBoard.hpp>
#include <sstream>
#include <MinMaxAgent.hpp>

std::string GraphGen::boardNode(const GameBoard &b, const std::string& data, const std::string& colour, const std::string id)
{
	return b.encodeString() + id + " [label = \""+ b.toString("", false) +"\\n"+data+"\", shape=\"box\", color=\""+colour+"\"]";
}

std::string GraphGen::graph(const GameBoard &b)
{
	std::stringstream ss;
	ss << "digraph {";

	MinMaxAgent agent(T_WHITE);
	agent.setStateEvaluatedCallback([&](const GameBoard& b, const GameBoard& p, float score, float alpha, float beta, size_t depth)
	{
		ss << p.encodeString() << (depth-1) << " -> " << b.encodeString() << depth << ";\n";
		TokenColour tc = ((depth%2==0) ? T_WHITE : T_RED);
		std::stringstream sbuff;
		std::stringstream snbg;
		snbg << depth;
		sbuff << (b.isEndGame() ? "T":"H") << " " << score << " " << alpha << " " << beta;
		ss << boardNode(b, sbuff.str(), (tc==T_RED) ? "red" : "white", snbg.str());
	});

	agent.calculateMove(b);

	ss << "}";
	return ss.str();
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
