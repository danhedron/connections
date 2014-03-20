#include "MainWindow.hpp"
#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>
#include "AIPlayerWorker.hpp"
#include <MinMaxAgent.hpp>

void MainWindow::queueAIMove(Agent *agent, TokenColour tc)
{
	setCurrentPlayer(tc);
	AIPlayerWorker* worker = new AIPlayerWorker(agent);
	connect(worker, SIGNAL(moveDecided(MoveResult*)), this, SLOT(makeMove(MoveResult*)));
	worker->startMove( gbw->gameBoard());
}

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags), gbw(nullptr), boardSize(5), redai(false),
	  redPlayerAgent(nullptr), whitePlayerAgent(nullptr)
{
	setMinimumSize(300, 300);

	QMenuBar *mb = menuBar();
	QMenu* game = mb->addMenu("&Game");
	QMenu* size = game->addMenu("&New");
	connect(size, SIGNAL(triggered(QAction*)), this, SLOT(resetGame()));

	size->addAction("&Tiny", this, SLOT(tinyGame()));
	size->addAction("&Small", this, SLOT(smallGame()));
	size->addAction("&Medium", this, SLOT(mediumGame()));
	size->addAction("&Normal", this, SLOT(normalGame()));
	size->addAction("&Large", this, SLOT(largeGame()));

	game->addAction("&AI Versus", this, SLOT(AIGame()));

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
	else {
		if(currentTurn == T_RED) {
			// ask AI player to do something.
			queueAIMove(whitePlayerAgent, T_WHITE);
		}
		else if(currentTurn == T_WHITE) {
			if(redai) {
				queueAIMove(redPlayerAgent, T_RED);
			} else {
				setCurrentPlayer(T_RED);
			}
		}
	}
}

void MainWindow::makeMove(MoveResult *m)
{
	if(m->player() == currentTurn) {
		makeMove(m->move().row, m->move().column);
	}
}

void MainWindow::tinyGame()
{
	boardSize = 2;
	resetGame();
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

void MainWindow::mediumGame()
{
	boardSize = 4;
	resetGame();
}

void MainWindow::normalGame()
{
	boardSize = 5;
	resetGame();
}

void MainWindow::largeGame()
{
	boardSize = 6;
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
	if(redPlayerAgent) {
		delete redPlayerAgent;
	}
	whitePlayerAgent = new MinMaxAgent(T_WHITE);

	if(redai) {
		redPlayerAgent = new MinMaxAgent(T_RED);

		queueAIMove(redPlayerAgent, T_RED);
	}

	setCurrentPlayer(T_RED);
}

void MainWindow::AIGame()
{
	redai = !redai;
	resetGame();
}

void MainWindow::playerClick(BoardIndex row, BoardIndex column)
{
	if(gbw->gameBoard()->getColour(row,column) == T_EMPTY) {
		if(currentTurn == T_RED && !redai) {
			makeMove(row, column);
		}
	}
}
