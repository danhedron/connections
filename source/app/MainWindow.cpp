#include "MainWindow.hpp"
MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{
	view = new QDeclarativeView();
	view->setSource(QUrl::fromLocalFile("../../asset/Board.qml"));
	setCentralWidget(view);
}
