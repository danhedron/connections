#include <GraphGen.hpp>
#include <GameBoard.hpp>
#include <sstream>
#include <MinMaxAgent.hpp>

PrepareImageCallback GraphGen::pic;

std::string baseatrib = "fontcolor=\"green\", fontname=\"times:bold\"";

std::string GraphGen::boardNode(const GameBoard &b, const std::string& data, const std::string& colour, const std::string id)
{
	std::string hash = b.encodeHash(true);
	if(pic) pic(b, hash);
	return hash + " [image = \""+ hash +".png\", "+baseatrib+", label=\""+data+"\", labelloc=\"b\", shape=\"box\", color=\""+colour+"\"]";
}

std::string GraphGen::graph(const GameBoard &b, unsigned int thinkDepth, unsigned int showDepth)
{
	if(showDepth == 0) showDepth = thinkDepth;
	std::stringstream ss;
	ss << "digraph {\n";
	ss << "imagepath = \"~/Projects/connections-build/graphs/\";\n";
	ss << "ranksep = 8.0\n";

	MinMaxAgent agent(T_WHITE, thinkDepth);
	agent.setStateEvaluatedCallback([&](const GameBoard& b, const GameBoard& p, float score, float alpha, float beta, size_t depth, char s)
	{
		if(depth > showDepth) return;
		ss << p.encodeHash(true) << " -> " << b.encodeHash(true) << ";\n";
		TokenColour tc = ((depth%2==0) ? T_WHITE : T_RED);
		std::stringstream sbuff;
		std::stringstream snbg;
		snbg << depth;
		sbuff << s << "\\n" << score << "\\n" << alpha << "\\n" << beta;
		ss << boardNode(b, sbuff.str(), (tc==T_RED) ? "red" : "white", snbg.str()) << "\n";
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
