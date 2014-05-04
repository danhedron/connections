#pragma once
#ifndef _APP_MAIN_WINDOW_HPP
#define _APP_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <QLabel>
#include <GameTypes.hpp>
#include <GameBoard.hpp>

class GameBoardWidget;
class MinMaxAgent;
class AIPlayerWorker;
class MoveResult;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	GameBoardWidget* gbw;

	BoardIndex boardSize;

	TokenColour currentTurn;

	QLabel* statusLabel;

	bool redai;

	MinMaxAgent* redPlayerAgent;

	MinMaxAgent* whitePlayerAgent;

	void queueAIMove(MinMaxAgent *agent, TokenColour tc);

	GameBoard startBoard;

	std::vector<GameBoard> history;

	AIPlayerWorker* currentWorker;

public:

	MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void setCurrentPlayer(TokenColour player);

	void setStartBoard(const GameBoard &b);

public slots:

	void makeMove(BoardIndex row, BoardIndex column);
	void makeMove(BoardIndex row, BoardIndex column, TokenColour tc);

private slots:

	void tinyGame();

	void smallGame();

	void mediumGame();

	void normalGame();

	void largeGame();

	void resetGame();

	void forfeitMove();

	void AIGame();

	void playerClick(BoardIndex row, BoardIndex column);

	void writeGraphviz();

	void writeStateImage();

	void printGameHistory();
};

#endif 
