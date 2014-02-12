#pragma once
#ifndef _GAMEBOARDWIDGET_HPP_
#define _GAMEBOARDWIDGET_HPP_
#include <QWidget>

class GameBoard;
class GameBoardWidget : public QWidget
{
	GameBoard* board;
public:
	
	GameBoard* gameBoard() const;
	
	void setGameBoard(GameBoard* b);
	
    virtual void paintEvent(QPaintEvent*);
};

#endif