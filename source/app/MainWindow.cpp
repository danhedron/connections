#include "MainWindow.hpp"
#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags), gbw(nullptr)
{
    setMinimumSize(300, 300);
	gbw = new GameBoardWidget;
    GameBoard* gb = new GameBoard(5);
    gb->putToken(1,0, T_RED);
    gb->putToken(1,1, T_WHITE);
    gbw->setGameBoard(gb);
	
	setCentralWidget(gbw);
	
}
