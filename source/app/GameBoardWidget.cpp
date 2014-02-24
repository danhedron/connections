#include "GameBoardWidget.hpp"
#include <game/GameBoard.hpp>
#include <QPainter>

void GameBoardWidget::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	QBrush red;
	red.setColor(Qt::red);
	QBrush white;
	white.setColor(Qt::white);
	
	QBrush back(Qt::darkGray);
	p.fillRect(0, 0, width(), height(), back);
	
	if(board) {
		for(size_t r = 0; r < board->getBoardLength(); ++r) {
            int rowoff = (board->getRowColour(r) == T_RED) ? 40 : 20;
			for(size_t c = 0; c < board->getRowSize(r) + 1; ++c) {
				if( c < board->getRowSize(r) ) {
					// Draw token
					auto s = board->getColour(r, c);
                    if( s != T_EMPTY ) {
                        bool vert = true;
                        if(board->getRowColour(r) == s) { vert = false; }
                        if(vert) {
                            p.fillRect(c * 40 + rowoff + 20, 5 + r * 20, 5, 35, s == T_RED ? Qt::red : Qt::white);
                        }
                        else {
                            p.fillRect(c * 40 + rowoff + 5, 20 + r * 20, 35, 5, s == T_RED ? Qt::red : Qt::white);
                        }
                    }
				}
                p.fillRect(c * 40 + rowoff, 20 + r * 20, 5, 5, board->getRowColour(r) == T_RED ? Qt::red : Qt::white);
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
}

