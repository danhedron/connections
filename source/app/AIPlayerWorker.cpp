#include "AIPlayerWorker.hpp"

AIPlayerWorker::AIPlayerWorker(Agent* agent)
    : agent(agent)
{
    this->moveToThread(&workerThread);
}

void AIPlayerWorker::startMove(GameBoard* gbw)
{
    this->moveToThread(&workerThread);
    connect(&workerThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(&workerThread, SIGNAL(started()), this, SLOT(decideNextMove()));
    gameBoard = gbw;
    workerThread.start();
}

void AIPlayerWorker::decideNextMove()
{
    Move m = agent->calculateMove(*gameBoard);
	emit moveDecided(new MoveResult(m, agent->colour(), this));
}
