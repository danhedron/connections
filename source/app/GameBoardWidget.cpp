#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QPainter>
#include <QMouseEvent>

QPoint GameBoardWidget::calculateTilePosition(BoardIndex row, BoardIndex col) const
{
	const int tilesize = 40;
	const int halftile = tilesize / 2;
	int rf = halftile;
	if(board->getRowColour(row) == T_RED) {
		rf = tilesize;
	}
	return QPoint(rf + col * tilesize, halftile + row * halftile);
}

void GameBoardWidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	QBrush red;
	red.setColor(Qt::red);
	QBrush white;
	white.setColor(Qt::white);
	
	QBrush back(Qt::darkGray);
	p.fillRect(0, 0, width(), height(), back);

	QPolygon tokenBack;
	tokenBack << QPoint(20, -10) << QPoint(30, -10)
			  << QPoint(40,   0) << QPoint(40,  10)
			  << QPoint(30,  20) << QPoint(20,  20)
			  << QPoint(10,  10) << QPoint(10,   0);
	p.setBrush(QBrush(Qt::black));
	
	if(board) {
		for(size_t r = 0; r < board->getBoardLength(); ++r) {
			for(size_t c = 0; c < board->getRowSize(r) + 1; ++c) {
				QPoint t = calculateTilePosition(r, c);
				p.setTransform(QTransform::fromTranslate(t.x(), t.y()));
				p.fillRect(0, 0, 10, 10, board->getRowColour(r) == T_RED ? Qt::red : Qt::white);
				if( c < board->getRowSize(r) ) {
					// Draw token
					auto s = board->getColour(r, c);
					auto color = s==T_RED?Qt::red : Qt::white;
                    if( s != T_EMPTY ) {
						p.drawPolygon(tokenBack);
                        bool vert = true;
                        if(board->getRowColour(r) == s) { vert = false; }
                        if(vert) {
							p.fillRect(20, - 10, 10, 30, color);
                        }
                        else {
							p.fillRect(10, 0, 30, 10, color);
                        }
                    }
				}
			}
		}
	}
}

GameBoard* GameBoardWidget::gameBoard() const
{
	return board;
}

void GameBoardWidget::setGameBoard(GameBoard* b)
{
	board = b;
	update();
}

void GameBoardWidget::mousePressEvent(QMouseEvent* e)
{

}
