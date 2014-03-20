#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QPainter>
#include <QMouseEvent>

const int leftMargin = 20;
const int tilesize = 40;
const int halftile = tilesize / 2;

QPointF GameBoardWidget::calculateTilePosition(BoardIndex row, BoardIndex col) const
{
	int rf = 0;
	if(board->getRowColour(row) == T_WHITE) {
		rf = halftile;
	}
	return QPointF(leftMargin + rf + col * tilesize, halftile + row * halftile);
}

QTransform GameBoardWidget::calculateBoardTransform() const
{
	float size = (leftMargin) + halftile * (board->getBoardLength());
	float scale = std::min(height()/size, width()/size);
	QTransform tScale = QTransform::fromScale(scale, scale);
	QTransform tOffset;
	if( height() > width() ) {
		tOffset = QTransform::fromTranslate(0.f, (height() - size*scale)/2.f);
	} else {
		tOffset = QTransform::fromTranslate((width() - size*scale)/2.f, 0.f);
	}
	return tScale * tOffset;
}

void GameBoardWidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	QBrush back(Qt::darkGray);
	p.fillRect(0, 0, width(), height(), back);
	auto btr = calculateBoardTransform();

	QPolygonF tokenBack;
	tokenBack << QPointF( -5, -15) << QPointF(  5, -15)
			  << QPointF( 15,  -5) << QPointF( 15,   5)
			  << QPointF(  5,  15) << QPointF( -5,  15)
			  << QPointF(-15,   5) << QPointF(-15,  -5);
	p.setBrush(QBrush(Qt::black));
	
	if(board) {
		for(size_t r = 0; r < board->getBoardLength(); ++r) {
			for(size_t c = 0; c < board->getRowSize(r) + 1; ++c) {
				QPointF t = calculateTilePosition(r, c);
				p.setTransform(btr);
				p.setTransform(QTransform::fromTranslate(t.x(), t.y()), true);
				if((c != 0 && c != board->getRowSize(r)) || board->getRowColour(r) == T_WHITE) {
					p.fillRect(QRectF(-25, -5, 10, 10), board->getRowColour(r) == T_RED ? Qt::red : Qt::white);
				}
				if( c < board->getRowSize(r) ) {
					// Draw token
					auto s = board->getColour(r, c);
					auto color = s==T_RED?Qt::red : Qt::white;
					if( s != T_EMPTY ) {
						p.drawPolygon(tokenBack);
						bool vert = true;
						if(board->getRowColour(r) == s) { vert = false; }
						if(vert) {
							p.fillRect(QRectF(-5, -15, 10, 30), color);
						}
						else {
							p.fillRect(QRectF(-15, -5, 30, 10), color);
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
	auto btr = calculateBoardTransform();
	if(board) {
		BoardIndex crow = 0;
		BoardIndex ccol = 0;
		int closest = std::numeric_limits<int>::max();
		for(size_t r = 0; r < board->getBoardLength(); ++r) {
			for(size_t c = 0; c < board->getRowSize(r); ++c) {
				QPointF p = btr.map(calculateTilePosition(r, c));
				auto d = (p - e->pos()).manhattanLength();
				if(d < closest) {
					crow = r; ccol = c;
					closest = d;
				}
			}
		}
		emit boardClicked(crow, ccol);
	}
}
