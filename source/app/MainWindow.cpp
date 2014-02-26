#include "MainWindow.hpp"
#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>
#include <MinMaxAgent.hpp>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags), gbw(nullptr), boardSize(5),
  redPlayerAgent(nullptr), whitePlayerAgent(nullptr)
{
	setMinimumSize(300, 300);

	QMenuBar *mb = menuBar();
	QMenu* game = mb->addMenu("&Game");
	QMenu* size = game->addMenu("&New");
	connect(size, SIGNAL(triggered(QAction*)), this, SLOT(resetGame()));

	size->addAction("&Normal", this, SLOT(normalGame()));
	size->addAction("&Small", this, SLOT(smallGame()));
	size->addAction("&Large", this, SLOT(largeGame()));

	game->addSeparator();
	game->addAction("&Quit", QApplication::instance(), SLOT(quit()), QKeySequence::Quit);

	QMenu* help = mb->addMenu("&Help");
	help->addAction("&About Qt", QGuiApplication::instance(), SLOT(aboutQt()));

	statusLabel = new QLabel();
	statusBar()->addWidget(statusLabel);

	gbw = new GameBoardWidget;
	
	setCentralWidget(gbw);

	normalGame();

	setCurrentPlayer(T_RED);

	connect(gbw, SIGNAL(boardClicked(BoardIndex,BoardIndex)), this, SLOT(playerClick(BoardIndex,BoardIndex)));
}

void MainWindow::makeMove(BoardIndex row, BoardIndex column)
{
	gbw->gameBoard()->putToken(row, column, currentTurn);
	gbw->update();
	if(gbw->gameBoard()->isEndGame()) {
		auto winner = gbw->gameBoard()->getWinner();

		QMessageBox mbx;
		if(winner == T_RED) {
			mbx.setText("Red Player Wins!");
		} else if (winner == T_WHITE) {
			mbx.setText("White Player Wins!");
		}
		else {
			mbx.setText("Nobody Wins!");
		}

		mbx.exec();

		resetGame();
	}
}

void MainWindow::setCurrentPlayer(TokenColour player)
{
	if(! gbw->gameBoard()->isEndGame()) {
		currentTurn = player;
		if(currentTurn == T_RED) {
			statusLabel->setText("Reds turn");
		}
		else {
			statusLabel->setText("Whites turn");
		}
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

	if(whitePlayerAgent) {
		delete whitePlayerAgent;
	}

	whitePlayerAgent = new MinMaxAgent(T_WHITE);
}

void MainWindow::playerClick(BoardIndex row, BoardIndex column)
{
	if(gbw->gameBoard()->getColour(row,column) == T_EMPTY) {
		if(currentTurn == T_RED) {
			makeMove(row, column);
			setCurrentPlayer(T_WHITE);
		}
		if(! gbw->gameBoard()->isEndGame()) {
			// ask AI player to do something.
			// TODO: move this into worker
			Move m = whitePlayerAgent->calculateMove(* gbw->gameBoard());
			makeMove(m.row, m.column);

			setCurrentPlayer(T_RED);
		}
	}
}
