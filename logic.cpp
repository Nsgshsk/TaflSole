#include "logic.h"
#include "constants.h"
#include "board.h"
#include "history.h"
#include "positionChecks.h"

bool isKingOnlyCell(size_t boardSize, size_t row, size_t col)
{
	if (isKingStartingPosition(boardSize, row, col))
		return true;
	else if (isCorner(row, col, boardSize))
		return true;
	else
		return false;
}

bool isKingOnlyCell(size_t boardSize, Position* move)
{
	return isKingOnlyCell(boardSize, move->x, move->y);
}

bool isEnemyCell(Board board, size_t size, char pieceType, size_t row, size_t col)
{
	if (isOutOfBounds(size, row, col))
		return true;

	char moveCell = typeOfCell(board, size, row, col);
	switch (pieceType)
	{
		case DEFENDER:
			if (isKingOnlyCell(size, row, col))
				return true;
		case KING:
			return moveCell == ATTACKER;

		case ATTACKER:
			if (isKingOnlyCell(size, row, col))
				return true;
			return moveCell == DEFENDER || moveCell == KING;
	}
}

bool isEnemyCell(Board board, size_t size, char pieceType, Position* move)
{
	return isEnemyCell(board, size, pieceType, move->x, move->y);
}

bool isTakenCell(Board board, size_t size, size_t row, size_t col)
{
	if (isOutOfBounds(size, row, col))
		return true;

	char cellType = typeOfCell(board, size, row, col);

	if (cellType == EMPTY_SPACE)
		return true;

	return false;
}

bool isTakenCell(Board board, size_t size, Position* move)
{
	return isTakenCell(board, size, move->x, move->y);
}

bool isValidMove(Board board, size_t size, char pieceType, Position* move)
{
	if (isOutOfBounds(size, move->x, move->y))
		return false;

	switch (pieceType)
	{
		case KING:
		case DEFENDER:
		case ATTACKER:
			return !isTakenCell(board, size, move) && !isEnemyCell(board, size, pieceType, move);
	}

	return false;
}

bool isCaptured(Board board, size_t size, char pieceType, Position* piece)
{
	if (isOutOfBounds(size, piece->x, piece->y))
		return false;

	// row += 1, col += 0
	bool isEnemy1 = isEnemyCell(board, size, pieceType, piece->x + 1, piece->y);
	// row += 0, col += 1
	bool isEnemy2 = isEnemyCell(board, size, pieceType, piece->x, piece->y + 1);
	// row += -1, col += 0
	bool isEnemy3 = isEnemyCell(board, size, pieceType, piece->x - 1, piece->y);
	// row += 0, col += -1
	bool isEnemy4 = isEnemyCell(board, size, pieceType, piece->x, piece->y - 1);

	switch (pieceType)
	{
		case ATTACKER:
		case DEFENDER:
			return isEnemy1 && isEnemy3 || isEnemy2 && isEnemy4;

		case KING:
			return isEnemy1 && isEnemy3 && isEnemy2 && isEnemy4;

	}

	return false;
}

bool movePiece(Board board, Position* piece, Position* move)
{

}