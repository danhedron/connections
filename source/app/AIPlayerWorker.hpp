#ifndef AIPLAYERWORKER_H
#define AIPLAYERWORKER_H

#include <QThread>
#include <MinMaxAgent.hpp>

class MoveResult : public QObject
{
    Move m;
	TokenColour p;

public:

	MoveResult(const Move& m, TokenColour c, QObject* parent)
		: QObject(parent), m(m), p(c)
    {}

    const Move& move() const
    { return m; }

	TokenColour player() const
	{ return p; }
};

class AIPlayerWorker : public QObject
{
    Q_OBJECT
    QThread workerThread;
    Agent* agent;
    GameBoard* gameBoard;
public:

    AIPlayerWorker(Agent* agent);

    void startMove(GameBoard *gbw);

public slots:

    void decideNextMove();

signals:

    void moveDecided(MoveResult*);
};

#endif // AIPLAYERWORKER_H
