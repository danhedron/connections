#pragma once
#ifndef _APP_MAIN_WINDOW_HPP
#define _APP_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <QLabel>
#include <GameTypes.hpp>

class GameBoardWidget;
class MainWindow : public QMainWindow
{
	Q_OBJECT

	GameBoardWidget* gbw;

	BoardIndex boardSize;

	TokenColour currentTurn;

	QLabel* statusLabel;

public:

	MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);

	void makeMove(BoardIndex row, BoardIndex column);

	void setCurrentPlayer(TokenColour player);

private slots:

	void smallGame();

	void normalGame();

	void largeGame();

	void resetGame();

	void playerClick(BoardIndex row, BoardIndex column);
};

#endif 
