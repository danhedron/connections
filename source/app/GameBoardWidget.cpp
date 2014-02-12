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
			int rowoff = (board->getRowSize(r) != board->getBoardLength()) ? 10 : 0;
			for(size_t c = 0; c < board->getRowSize(r) + 1; ++c) {
				if( c < board->getRowSize(r) ) {
					// Draw token
					auto s = board->getColour(r, c);
				}
				p.fillRect(c * 20 + rowoff, r * 20, 5, 5, board->getRowColour(r) == T_RED ? Qt::red : Qt::white);
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

