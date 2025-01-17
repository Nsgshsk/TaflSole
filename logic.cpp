/**
*
* Solution to course project # 7
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
*
* @author Lyubomir Topalski
* @idnumber 8MI0600446
* @compiler VC
*
* Contains core game logic
*
*/

#include "board.h"
#include "constants.h"
#include "history.h"
#include "logic.h"
#include "positionChecks.h"

const size_t ARRAY_SIZE = 4;
const size_t ENEMY_TYPE_ARRAY_SIZE = 2;

Position* createPosition(size_t x, size_t y)
{
	Position* pos = new Position;
	pos->x = x;
	pos->y = y;
	return pos;
}

void deletePosition(Position* pos)
{
	delete pos;
}

bool isKingOnlyCell(size_t boardSize, size_t row, size_t col)
{
	if (isKingStartingPosition(boardSize, row, col))
		return true;
	else if (isCorner(boardSize, row, col))
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
		return false;

	char moveCell = typeOfCell(board, size, row, col);
	switch (pieceType)
	{
	case DEFENDER:
		if (moveCell == KING)
			return false;
		if (isKingOnlyCell(size, row, col))
			return true;
	case KING:
		return moveCell == ATTACKER;

	case ATTACKER:
		if (isKingOnlyCell(size, row, col))
			return true;
		return moveCell == DEFENDER || moveCell == KING;
	}
	return true;
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

bool isMovementInBounds(Board board, size_t size, Position* piece, Position* move, char pieceType)
{
	if (isOutOfBounds(size, piece->x, piece->y))
		return false;
	if (isOutOfBounds(size, move->x, move->y))
		return false;

	bool rowEqual = piece->x == move->x,
		colEqual = piece->y == move->y;

	if (rowEqual && colEqual)
		return false;

	size_t tmp1, tmp2;
	if (rowEqual)
	{
		if (piece->y < move->y)
		{
			tmp1 = piece->y;
			tmp2 = move->y;
		}
		else
		{
			tmp2 = piece->y;
			tmp1 = move->y;
		}
		for (size_t c = tmp1 + 1; c < tmp2; c++)
		{
			if (isTakenCell(board, size, move->x, c) ||
				(isEnemyCell(board, size, pieceType, move->x, c) && !isKingStartingPosition(size, move->x, c)))
				return false;
		}
		return true;
	}
	else if (colEqual)
	{
		if (piece->x < move->x)
		{
			tmp1 = piece->x;
			tmp2 = move->x;
		}
		else
		{
			tmp2 = piece->x;
			tmp1 = move->x;
		}
		for (size_t r = tmp1 + 1; r < tmp2; r++)
		{
			if (isTakenCell(board, size, r, move->y) ||
				(isEnemyCell(board, size, pieceType, r, move->y) && !isKingStartingPosition(size, r, move->y)))
				return false;
		}
		return true;
	}

	return false;
}

bool isValidMove(Board board, size_t size, char pieceType, Position* piece, Position* move)
{
	if (isOutOfBounds(size, move->x, move->y))
		return false;

	switch (pieceType)
	{
	case KING:
	case DEFENDER:
	case ATTACKER:
		return !isTakenCell(board, size, move) && !isEnemyCell(board, size, pieceType, move)
			&& isMovementInBounds(board, size, piece, move, pieceType);
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

	if (pieceType == KING)
	{
		bool kingOnlyCellArr[ARRAY_SIZE]{};
		bool edgeCellArr[ARRAY_SIZE]{};

		// row += 1, col += 0
		kingOnlyCellArr[0] = isKingOnlyCell(size, piece->x + 1, piece->y);
		// row += 0, col += 1
		kingOnlyCellArr[1] = isKingOnlyCell(size, piece->x, piece->y + 1);
		// row += -1, col += 0
		kingOnlyCellArr[2] = isKingOnlyCell(size, piece->x - 1, piece->y);
		// row += 0, col += -1
		kingOnlyCellArr[3] = isKingOnlyCell(size, piece->x, piece->y - 1);

		// row += 1, col += 0
		edgeCellArr[0] = isOutOfBounds(size, piece->x + 1, piece->y);
		// row += 0, col += 1
		edgeCellArr[1] = isOutOfBounds(size, piece->x, piece->y + 1);
		// row += -1, col += 0
		edgeCellArr[2] = isOutOfBounds(size, piece->x - 1, piece->y);
		// row += 0, col += -1
		edgeCellArr[3] = isOutOfBounds(size, piece->x, piece->y - 1);

		bool once = false;
		for (size_t i = 0; i < ARRAY_SIZE; i++)
		{
			if (!isEnemyArr[i] && !once)
			{
				isEnemyArr[i] = kingOnlyCellArr[i] || edgeCellArr[i];
				once = true;
			}
		}
	}
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

bool movePiece(Board board, size_t size, Position* piece, Position* move, char& pieceType, bool player)
{
	if (isOutOfBounds(size, move->x, move->y))
		return false;

	pieceType = typeOfCell(board, size, piece->x, piece->y);

	if (!player)
	{
		if (pieceType != ATTACKER)
			return false;
	}
	else
	{
		if (pieceType == ATTACKER)
			return false;
	}

	if (!isValidMove(board, size, pieceType, piece, move))
		return false;

	bool isKingOnly = isKingOnlyCell(size, piece);

	bool result = changeCell(board, size, move->x, move->y, pieceType);
	return result && changeCell(board, size, piece->x, piece->y, isKingOnly ? END_POINT : EMPTY_SPACE);
}

bool fillTakenArr(Board board, size_t size, Position*& taken, size_t takenSize,
	bool isEnemyArr[ARRAY_SIZE], char pieceType, Position* move, bool& isGameOver)
{
	if (takenSize == 0)
		return false;

	size_t takenIndex = 0;
	taken = new Position[takenSize];

	for (size_t i = 0; i < ARRAY_SIZE; i++)
	{
		if (!takenSize)
			break;

		if (isEnemyArr[i])
		{
			Position enemy = *move;
			switch (i)
			{
			case 0:
				enemy.x += 1;
				break;
			case 1:
				enemy.y += 1;
				break;
			case 2:
				enemy.x -= 1;
				break;
			case 3:
				enemy.y -= 1;
				break;
			}
			char enemyType = typeOfCell(board, size, enemy.x, enemy.y);
			if (isCaptured(board, size, enemyType, &enemy))
			{
				if (enemyType == KING)
					isGameOver = true;

				if (!capturePiece(board, size, &enemy))
					return false;

				taken[takenIndex] = enemy;
				takenIndex++;
				takenSize--;
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

bool checkEnemieCaptured(const Board board, size_t size, Position* move, size_t location)
{
	Position enemy = *move;
	switch (location)
	{
	case 0:
		enemy.x += 1;
		break;
	case 1:
		enemy.y += 1;
		break;
	case 2:
		enemy.x -= 1;
		break;
	case 3:
		enemy.y -= 1;
		break;
	}
	char enemyType = typeOfCell(board, size, enemy.x, enemy.y);
	return isCaptured(board, size, enemyType, &enemy);
}

bool moveOperation(HistoryStack& history, Board board, size_t size, Position* piece, Position* move, bool& isGameOver, bool player, size_t& playerScore)
{
	char pieceType;
	if (!movePiece(board, size, piece, move, pieceType, player))
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
		if (isEnemyArr[i] && checkEnemieCaptured(board, size, move, i))
			takenSize++;
	}

	if (takenSize > 0 && fillTakenArr(board, size, taken, takenSize, isEnemyArr, pieceType, move, isGameOver))
	{
		saveMove(history, piece, move, taken, takenSize);
		playerScore += takenSize;
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

	Position* working = previous->move;
	char pieceType;
	if (!movePiece(board, size, previous->move, previous->piece, pieceType, !player))
		return false;

	if (previous->takenSize > 0)
	{
		pieceType = player ? DEFENDER : ATTACKER;
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