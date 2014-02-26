#pragma once
#ifndef _GAMEBOARDWIDGET_HPP_
#define _GAMEBOARDWIDGET_HPP_
#include <QWidget>
#include <GameTypes.hpp>

class GameBoard;
class GameBoardWidget : public QWidget
{
	Q_OBJECT

	GameBoard* board;

    QPointF calculateTilePosition(BoardIndex row, BoardIndex col) const;

    QTransform calculateBoardTransform() const;
public:
	
	GameBoard* gameBoard() const;
	
	void setGameBoard(GameBoard* b);
	
    virtual void paintEvent(QPaintEvent*);

	virtual void mousePressEvent(QMouseEvent*);

signals:

	void boardClicked(BoardIndex row, BoardIndex col);
};

#endif
