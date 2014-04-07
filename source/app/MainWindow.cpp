#include "MainWindow.hpp"
#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <game/GraphGen.hpp>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include "AIPlayerWorker.hpp"
#include <MinMaxAgent.hpp>
#include <QTextStream>

void MainWindow::queueAIMove(Agent *agent, TokenColour tc)
{
	setCurrentPlayer(tc);
	AIPlayerWorker* worker = new AIPlayerWorker(agent);
	connect(worker, SIGNAL(moveDecided(MoveResult*)), this, SLOT(makeMove(MoveResult*)));
	worker->startMove( gbw->gameBoard());
}

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags), gbw(nullptr), boardSize(5), redai(false),
	  redPlayerAgent(nullptr), whitePlayerAgent(nullptr), startBoard(5)
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

	QMenu* state = mb->addMenu("&State");

	state->addAction("&Forfeit move", this, SLOT(forfeitMove()));
	state->addSeparator();
	state->addAction("&Print History", this, SLOT(printGameHistory()));
	state->addAction("Write &Graph", this, SLOT(writeGraphviz()));
	state->addAction("Write &Image", this, SLOT(writeStateImage()));


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
	history.push_back(*gbw->gameBoard());
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

		redai = false;
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
		QString currentName = "White";
		QString currentStatus = "Thinking";
		if(currentTurn == T_RED) {
			currentName = "Red";
			if(! redai) {
				currentStatus = "Waiting for input";
			}
		}
		statusLabel->setText(QString("%1's Turn (%2)")
							 .arg(currentName).arg(currentStatus));
	}
}

void MainWindow::setStartBoard(const GameBoard &b)
{
	startBoard = b;
	resetGame();
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
	history.clear();

	delete gbw->gameBoard();
	GameBoard* gb = nullptr;
	if(startBoard.getRunSize() == boardSize) {
		gb = new GameBoard(startBoard);
	}
	else {
		gb = new GameBoard(boardSize);
	}
	gbw->setGameBoard(gb);

	if(whitePlayerAgent) {
		delete whitePlayerAgent;
	}
	if(redPlayerAgent) {
		delete redPlayerAgent;
	}

	int thinksize = gb->getRunSize()*gb->getRunSize()*4;
	if(boardSize > 4) thinksize = gb->getRunSize();

	whitePlayerAgent = new MinMaxAgent(T_WHITE, thinksize);

	if(redai) {
		redPlayerAgent = new MinMaxAgent(T_RED, thinksize);

		queueAIMove(redPlayerAgent, T_RED);
	}

	setCurrentPlayer(T_RED);
}

void MainWindow::forfeitMove()
{
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

#include <iostream>
#include <QPainter>

void MainWindow::writeGraphviz()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(tr("Graphviz file (*.gv)"));
	if(dialog.exec()) {
		QFile f(dialog.selectedFiles()[0]);
		f.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream out(&f);
		GameBoard b(*gbw->gameBoard());

		QString fdir = QFileInfo(dialog.selectedFiles()[0]).dir().path();

		GraphGen::setPrepareImageCallback([&](const GameBoard& b, const std::string& fname) {
			QFile imfile(fdir + "/" + QString::fromStdString(fname) + ".png");
			if(!imfile.exists()) {
				QImage image(100, 100, QImage::Format_RGB888);
				QPainter imp(&image);

				GameBoardWidget::paintBoard(&b, imp);

				image.save(imfile.fileName());

				std::cout << "Wrote image " << imfile.fileName().toStdString() << std::endl;
			}
		});

		out << QString::fromStdString(GraphGen::graph(b, 3*2, 4));
	}
}

void MainWindow::writeStateImage()
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(tr("PNG image (*.png)"));
	if(dialog.exec()) {
		QImage image(512, 512, QImage::Format_RGB888);
		QPainter imp(&image);
		GameBoardWidget::paintBoard(gbw->gameBoard(), imp);
		image.save(dialog.selectedFiles()[0]);
	}
}

void MainWindow::printGameHistory()
{
	for(const GameBoard& g : history) {
		std::cout << g.encodeString() << std::endl;
	}
}
