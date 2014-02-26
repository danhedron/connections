#include "MainWindow.hpp"
#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags), gbw(nullptr), boardSize(5)
{
	setMinimumSize(300, 300);

	QMenuBar *mb = menuBar();
	QMenu* game = mb->addMenu("&Game");
	QMenu* size = game->addMenu("&New");
	connect(size, SIGNAL(triggered(QAction*)), this, SLOT(resetGame()));

	size->addAction("&Normal", this, SLOT(normalGame()));
	size->addAction("&Small", this, SLOT(smallGame()));
	size->addAction("&Large", this, SLOT(largeGame()));

	statusLabel = new QLabel();
	statusBar()->addWidget(statusLabel);

	gbw = new GameBoardWidget;
	
	setCentralWidget(gbw);

	normalGame();

	auto gb = gbw->gameBoard();
	gb->putToken(0,0, T_RED);
	gb->putToken(0,1, T_RED);
	gb->putToken(0,2, T_RED);
	gb->putToken(1,0, T_WHITE);
	gb->putToken(1,1, T_WHITE);
	gb->putToken(1,2, T_WHITE);
	gb->putToken(1,3, T_WHITE);

	setCurrentPlayer(T_RED);

	connect(gbw, SIGNAL(boardClicked(BoardIndex,BoardIndex)), this, SLOT(playerClick(BoardIndex,BoardIndex)));
}

void MainWindow::makeMove(BoardIndex row, BoardIndex column)
{
	gbw->gameBoard()->putToken(row, column, currentTurn);
	gbw->update();
}

void MainWindow::setCurrentPlayer(TokenColour player)
{
	currentTurn = player;
	if(currentTurn == T_RED) {
		statusLabel->setText("Reds turn");
	}
	else {
		statusLabel->setText("Whites turn");
	}
}

void MainWindow::smallGame()
{
	boardSize = 3;
	resetGame();
}

void MainWindow::normalGame()
{
	boardSize = 5;
	resetGame();
}

void MainWindow::largeGame()
{
	boardSize = 5;
	resetGame();
}

void MainWindow::resetGame()
{
	delete gbw->gameBoard();
	GameBoard* gb = new GameBoard(boardSize);
	gbw->setGameBoard(gb);
}

void MainWindow::playerClick(BoardIndex row, BoardIndex column)
{
	if(currentTurn == T_RED) {
		makeMove(row, column);
	}
	else {
		makeMove(row, column);
	}
	// ask AI player to do something.
}
