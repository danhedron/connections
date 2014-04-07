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

	static QPointF calculateTilePosition(const GameBoard *board, BoardIndex row, BoardIndex col);

	static QTransform calculateBoardTransform(const GameBoard *board, QPaintDevice *d);
public:

	GameBoardWidget(QWidget *parent);
	
	GameBoard* gameBoard() const;
	
	void setGameBoard(GameBoard* b);
	
	virtual void paintEvent(QPaintEvent*);

	virtual void mousePressEvent(QMouseEvent*);

	static void paintBoard(const GameBoard *board, QPainter& p);

signals:

	void boardClicked(BoardIndex row, BoardIndex col);
};

#endif
