#include "logic.h"
#include "constants.h"
#include "board.h"
#include "history.h"
#include "positionChecks.h"

const size_t ARRAY_SIZE = 4;
const size_t ENEMY_TYPE_ARRAY_SIZE = 2;

struct Position;
struct MoveNode;

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

	return cellType != EMPTY_SPACE && cellType != END_POINT;
}

bool isTakenCell(Board board, size_t size, Position* move)
{
	return isTakenCell(board, size, move->x, move->y);
}

bool inMovementBounds(size_t boardSize, size_t pieceRow, size_t pieceCol, size_t row, size_t col)
{
	if (isOutOfBounds(boardSize, pieceRow, pieceCol))
		return false;
	if (isOutOfBounds(boardSize, row, col))
		return false;

	if (pieceRow == row)
		return true;

	if (pieceCol == col)
		return true;

	return false;
}

bool inMovementBounds(size_t boardSize, Position* piece, Position* move)
{
	return inMovementBounds(boardSize, piece->x, piece->y, move->x, move->y);
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

void fillIsEnemyArr(Board board, size_t size, char pieceType, Position* piece, bool isEnemyArr[ARRAY_SIZE])
{
	// row += 1, col += 0
	isEnemyArr[0] = isEnemyCell(board, size, pieceType, piece->x + 1, piece->y);
	// row += 0, col += 1
	isEnemyArr[1] = isEnemyCell(board, size, pieceType, piece->x, piece->y + 1);
	// row += -1, col += 0
	isEnemyArr[2] = isEnemyCell(board, size, pieceType, piece->x - 1, piece->y);
	// row += 0, col += -1
	isEnemyArr[3] = isEnemyCell(board, size, pieceType, piece->x, piece->y - 1);
}

bool isCaptured(Board board, size_t size, char pieceType, Position* piece)
{
	if (isOutOfBounds(size, piece->x, piece->y))
		return false;
	
	// Boolean array to store the information is the neighbouring piece an enemy
	bool isEnemyArr[ARRAY_SIZE]{};
	fillIsEnemyArr(board, size, pieceType, piece, isEnemyArr);

	switch (pieceType)
	{
		case ATTACKER:
		case DEFENDER:
			return isEnemyArr[0] && isEnemyArr[2] || isEnemyArr[1] && isEnemyArr[3];

		case KING:
			return isEnemyArr[0] && isEnemyArr[2] && isEnemyArr[1] && isEnemyArr[3];
	}

	return false;
}

bool capturePiece(Board board, size_t size, size_t row, size_t col)
{
	if (isOutOfBounds(size, row, col))
		return false;

	return changeCell(board, size, row, col, EMPTY_SPACE);
}

bool capturePiece(Board board, size_t size, Position* piece)
{
	return capturePiece(board, size, piece->x, piece->y);
}

bool movePiece(Board board, size_t size, Position* piece, Position* move, char &pieceType)
{
	if (isOutOfBounds(size, move->x, move->y))
		return false;

	pieceType = typeOfCell(board, size, piece->x, piece->y);

	if (!isValidMove(board, size, pieceType, move))
		return false;

	return changeCell(board, size, move->x, move->y, pieceType);
}

void fillEnemyTypeArr(char pieceType, char enemyType[ENEMY_TYPE_ARRAY_SIZE])
{
	switch (pieceType)
	{
		case ATTACKER:
			enemyType[1] = KING;
		case DEFENDER:
		case KING:
			enemyType[0] = pieceType == ATTACKER ? DEFENDER : ATTACKER;
			break;
	}
}

bool fillTakenArr(Board board, size_t size, Position*& taken, size_t takenSize, 
	bool isEnemyArr[ARRAY_SIZE], char pieceType, Position* move, bool& isGameOver)
{
	size_t takenIndex = 0;
	taken = new Position[takenSize];
	char enemyType[ENEMY_TYPE_ARRAY_SIZE]{};
	fillEnemyTypeArr(pieceType, enemyType);

	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		if (isEnemyArr[i])
		{
			Position enemy = *move;
			switch (i)
			{
				case 0:
					move->x += 1;
					break;
				case 1:
					move->y += 1;
					break;
				case 2:
					move->x -= 1;
					break;
				case 3:
					move->y -= 1;
					break;
			}
			for (size_t i = 0; i < ENEMY_TYPE_ARRAY_SIZE; i++)
			{
				if (isCaptured(board, size, enemyType[i], &enemy))
				{
					if (enemyType[i] == KING)
						isGameOver = true;

					if (!capturePiece(board, size, &enemy))
						return false;
					taken[takenIndex] = enemy;
				}

				if (enemyType[1] != KING)
					break;
			}
		}
	}
	return true;
}

bool isGameOverCondition(size_t boardSize, Position* move)
{
	if (isOutOfBounds(boardSize, move->x, move->y))
		return false;

	return isCorner(boardSize, move->x, move->y);
}

bool moveOperation(HistoryStack& history, Board board, size_t size, Position* piece, Position* move, bool& isGameOver)
{
	char pieceType;
	if (!movePiece(board, size, piece, move, pieceType))
		return false;

	if (isGameOverCondition(size, move))
		isGameOver = true;

	// Boolean array to store the information is the neighbouring piece an enemy
	bool isEnemyArr[ARRAY_SIZE]{};
	fillIsEnemyArr(board, size, pieceType, move, isEnemyArr);

	Position* taken = nullptr;
	size_t takenSize = 0;
	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		if (isEnemyArr[i])
			takenSize++;
	}

	if (takenSize > 0 && fillTakenArr(board, size, taken, takenSize, isEnemyArr, pieceType, move, isGameOver))
	{
		saveMove(history, piece, move, taken, takenSize);
		return true;
	}

	saveMove(history, piece, move);
	return true;
}

bool backOperation(HistoryStack& history, Board board, size_t size, bool player, size_t& playerScore)
{
	MoveNode* previous = backMove(history);
	if (previous == nullptr)
		return false;

	char pieceType = player ? DEFENDER : ATTACKER;

	Position* working = previous->move;
	changeCell(board, size, working->x, working->y, EMPTY_SPACE);

	working = previous->piece;
	changeCell(board, size, working->x, working->y, pieceType);

	if (previous->takenSize > 0)
	{
		pieceType = !player ? DEFENDER : ATTACKER;
		working = previous->taken;
		for (size_t i = 0; i < previous->takenSize; i++)
		{
			changeCell(board, size, working[i].x, working[i].y, pieceType);
		}
		playerScore -= previous->takenSize;
	}

	deleteMoveNode(previous);
	return true;
}