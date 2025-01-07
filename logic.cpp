#include "logic.h"
#include "constants.h"
#include "board.h"
#include "history.h"
#include "positionChecks.h"

bool isKingOnlyCell(size_t boardSize, Position* move)
{
	if (isKingStartingPosition(move->x, move->y, boardSize))
		return true;
	else if (isCorner(move->x, move->y, boardSize))
		return true;
	else
		return false;
}

bool isEnemyCell(Board board, size_t size, char pieceType, Position* move)
{
	char moveCell = typeOfCell(board, size, move->x, move->y);
	switch (pieceType)
	{
		case DEFENDER:
			if (isKingOnlyCell(size, move))
				return true;
		case KING:
			return moveCell == ATTACKER;

		case ATTACKER:
			if (isKingOnlyCell(size, move))
				return true;
			return moveCell == DEFENDER || moveCell == KING;
	}
}

bool isValidMove(Board board, size_t size, char pieceType, Position* move)
{
	if (isOutOfBounds(move->x, move->y, size))
		return false;

	switch (pieceType)
	{
		case KING:
			if (isKingOnlyCell(size, move))
				return true;
		case DEFENDER:
			if 
		case ATTACKER:

		default:
			return false;
	}

	return true;
}

bool movePiece(Board board, Position* piece, Position* move)
{

}