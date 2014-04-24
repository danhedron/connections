#include "AIPlayerWorker.hpp"

Q_DECLARE_METATYPE(BoardIndex)
Q_DECLARE_METATYPE(TokenColour)

AIPlayerWorker::AIPlayerWorker(Agent* agent)
	: _agent(agent)
{

	qRegisterMetaType<BoardIndex>("BoardIndex");
	qRegisterMetaType<TokenColour>("TokenColour");

	start();
	moveToThread(this);
}

void AIPlayerWorker::startMove(GameBoard* board)
{
	_board = board;
	QMetaObject::invokeMethod(this, "decideNextMove");
}

void AIPlayerWorker::decideNextMove()
{
	Move m = _agent->calculateMove(*_board);
	emit moveDecided(m.row, m.column, _agent->colour());
}
