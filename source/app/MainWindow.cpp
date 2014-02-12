#include "MainWindow.hpp"
#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags), gbw(nullptr)
{
	gbw = new GameBoardWidget;
	
	gbw->setGameBoard(new GameBoard(5));
	
	setCentralWidget(gbw);
	
}
