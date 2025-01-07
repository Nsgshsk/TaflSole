#include "board.h"
#include "positionChecks.h"
#include "constants.h"

void allocateBoardMemory(Board& board, size_t size)
{
	board = new char* [size];
	for (size_t row = 0; row < size; row++)
	{
		board[row] = new char[size];
	}
}

void unallocateBoardMemory(Board board, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		delete[] board[row];
	}
	delete[] board;
}

bool isAttackersStartingPosition(size_t row, size_t col, size_t boardSize)
{
	size_t middle = boardSize / 2;
	
	if (row == 0 || row == boardSize - 1)
		return col > 2 && col < boardSize - 3;

	else if (row == 1 || row == boardSize - 2)
		return col == middle;

	else if (col == 0 || col == boardSize - 1)
		return row > 2 && row < boardSize - 3;

	else if (col == 1 || col == boardSize - 2)
		return row == middle;

	return false;
}

bool isDefendersStartingPosition(size_t row, size_t col, size_t boardSize)
{
	size_t middle = boardSize / 2;
	bool result = false;

	if (row == col && row == middle)
		return false;

	switch (boardSize)
	{
		case SMALL:
			if (row >= 2 && row <= boardSize - 3)
				result = result || col == middle;

			if (col >= 2 && col <= boardSize - 3)
				result = result || row == middle;
			break;

		case MEDIUM:
			if (row == col)
				result = result || row == middle - 1 || row == middle + 1;

			if (row == middle - 1 && col == middle + 1)
				result = result || true;

			if (row == middle + 1 && col == middle - 1)
				result = result || true;

		case BIG:
			if (row > 2 && row < boardSize - 3)
				result = result || col == middle;

			if (col > 2 && col < boardSize - 3)
				result = result || row == middle;
			break;
	}
	return result;
}

void initializePieces(Board board, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 0; col < size; col++)
		{
			if (isCorner(row, col, size))
				board[row][col] = END_POINT;

			else if (isAttackersStartingPosition(row, col, size))
				board[row][col] = ATTACKER;

			else if (isDefendersStartingPosition(row, col, size))
				board[row][col] = DEFENDER;

			else if (isKingStartingPosition(row, col, size))
				board[row][col] = KING;

			else
				board[row][col] = EMPTY_SPACE;
		}
	}
}

bool initializeBoard(Board board, size_t size)
{
	switch (size)
	{
		case SMALL:
			initializePieces(board, SMALL);
			break;

		case MEDIUM:
			initializePieces(board, MEDIUM);
			break;

		case BIG:
			initializePieces(board, BIG);
			break;

		default:
			return false;
	}
	return true;
}

bool newBoard(Board& board, size_t oldSize, size_t newSize)
{
	if (board != nullptr)
		unallocateBoardMemory(board, oldSize);

	allocateBoardMemory(board, newSize);
	return initializeBoard(board, newSize);
}

void closeBoard(Board board, size_t size)
{
	unallocateBoardMemory(board, size);
}

bool changeCell(Board board, size_t size, size_t row, size_t col, char cellType)
{
	if (isOutOfBounds(row, col, size))
		return false;

	board[row][col] = cellType;

	return true;
}

char typeOfCell(Board board, size_t size, size_t row, size_t col)
{
	if (isOutOfBounds(row, col, size))
		return -1;

	return board[row][col];
}