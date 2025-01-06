#include "board.h"
#include "positionChecks.h"

enum BoardSizes
{
	SMALL = 9,
	MEDIUM = 11,
	BIG = 13
};

enum BoardChars
{
	EMPTY_SPACE = '-',
	ATTACKER = 'A',
	DEFENDER = 'D',
	KING = 'K',
	END_POINT = 'X'
};

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

bool isAttackersStartingPosition(size_t row, size_t col, size_t size)
{
	size_t middle = size / 2;
	
	if (row == 0 || row == size - 1)
		return col > 2 && col < size - 3;

	else if (row == 1 || row == size - 2)
		return col == middle;

	else if (col == 0 || col == size - 1)
		return row > 2 && row < size - 3;

	else if (col == 1 || col == size - 2)
		return row == middle;

	return false;
}

bool isDefendersStartingPosition(size_t row, size_t col, size_t size)
{
	size_t middle = size / 2;
	bool result = false;

	if (row == col && row == middle)
		return false;

	switch (size)
	{
		case SMALL:
			if (row >= 2 && row <= size - 3)
				result = result || col == middle;

			if (col >= 2 && col <= size - 3)
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
			if (row > 2 && row < size - 3)
				result = result || col == middle;

			if (col > 2 && col < size - 3)
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