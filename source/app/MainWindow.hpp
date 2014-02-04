#pragma once
#ifndef _APP_MAIN_WINDOW_HPP
#define _APP_MAIN_WINDOW_HPP
#include <QMainWindow>
#include <QDeclarativeView>

class MainWindow : public QMainWindow
{
	
	QDeclarativeView* view;
	
public:
	
	MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
};

#endif 
