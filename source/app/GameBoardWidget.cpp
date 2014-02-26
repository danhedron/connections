#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QPainter>
#include <QMouseEvent>

const int leftMargin = 20;
const int tilesize = 40;
const int halftile = tilesize / 2;

QPoint GameBoardWidget::calculateTilePosition(BoardIndex row, BoardIndex col) const
{
	int rf = halftile;
	if(board->getRowColour(row) == T_RED) {
		rf = tilesize;
	}
	return QPoint(leftMargin + rf + col * tilesize, halftile + row * halftile);
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
	QBrush red;
	red.setColor(Qt::red);
	QBrush white;
	white.setColor(Qt::white);
	
	QBrush back(Qt::darkGray);
    p.fillRect(0, 0, width(), height(), back);
    auto btr = calculateBoardTransform();

	QPolygon tokenBack;
	tokenBack << QPoint( -5, -15) << QPoint(  5, -15)
			  << QPoint( 15,  -5) << QPoint( 15,   5)
			  << QPoint(  5,  15) << QPoint( -5,  15)
			  << QPoint(-15,   5) << QPoint(-15,  -5);
	p.setBrush(QBrush(Qt::black));
	
	if(board) {
		for(size_t r = 0; r < board->getBoardLength(); ++r) {
			for(size_t c = 0; c < board->getRowSize(r) + 1; ++c) {
				QPoint t = calculateTilePosition(r, c);
                p.setTransform(btr);
                p.setTransform(QTransform::fromTranslate(t.x(), t.y()), true);
				p.fillRect(-25, -5, 10, 10, board->getRowColour(r) == T_RED ? Qt::red : Qt::white);
				if( c < board->getRowSize(r) ) {
					// Draw token
					auto s = board->getColour(r, c);
					auto color = s==T_RED?Qt::red : Qt::white;
                    if( s != T_EMPTY ) {
						p.drawPolygon(tokenBack);
                        bool vert = true;
                        if(board->getRowColour(r) == s) { vert = false; }
                        if(vert) {
							p.fillRect(-5, -15, 10, 30, color);
                        }
                        else {
							p.fillRect(-15, -5, 30, 10, color);
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
	if(board) {
		BoardIndex crow = 0;
		BoardIndex ccol = 0;
		int closest = std::numeric_limits<int>::max();
		for(size_t r = 0; r < board->getBoardLength(); ++r) {
			for(size_t c = 0; c < board->getRowSize(r); ++c) {
				QPoint p = calculateTilePosition(r, c);
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
