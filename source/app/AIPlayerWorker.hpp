#ifndef AIPLAYERWORKER_H
#define AIPLAYERWORKER_H

#include <QThread>
#include <MinMaxAgent.hpp>

class MoveResult : public QObject
{
    Move m;

public:

    MoveResult(const Move& m) : m(m)
    {}

    const Move& move() const
    { return m; }
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
