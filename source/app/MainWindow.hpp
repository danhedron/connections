#pragma once
#ifndef _APP_MAIN_WINDOW_HPP
#define _APP_MAIN_WINDOW_HPP
#include <QMainWindow>

class GameBoardWidget;
class MainWindow : public QMainWindow
{
	GameBoardWidget* gbw;
public:
	
	MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
};

#endif 
