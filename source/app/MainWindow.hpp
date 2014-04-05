#pragma once
#ifndef _APP_MAIN_WINDOW_HPP
#define _APP_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <QLabel>
#include <GameTypes.hpp>
#include <GameBoard.hpp>

class GameBoardWidget;
class Agent;
class MoveResult;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	GameBoardWidget* gbw;

	BoardIndex boardSize;

	TokenColour currentTurn;

	QLabel* statusLabel;

	bool redai;

	Agent* redPlayerAgent;

	Agent* whitePlayerAgent;

	void queueAIMove(Agent* agent, TokenColour tc);

	GameBoard startBoard;

	std::vector<GameBoard> history;

public:

	MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void setCurrentPlayer(TokenColour player);

	void setStartBoard(const GameBoard &b);

public slots:

	void makeMove(BoardIndex row, BoardIndex column);
	void makeMove(MoveResult*);

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
