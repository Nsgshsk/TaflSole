#include "positionChecks.h"

bool isOutOfBounds(size_t row, size_t col, size_t size)
{
	if (row < 0 || row >= size)
		return true;

	else if (col < 0 || col >= size)
		return true;

	return false;
}

bool isCorner(size_t row, size_t col, size_t size)
{
	if (row == 0)
		return col == 0 || col == size - 1;

	else if (row == size - 1)
		return col == 0 || col == size - 1;

	return false;
}

bool isKingStartingPosition(size_t row, size_t col, size_t size)
{
	size_t middle = size / 2;
	return row == middle && col == middle;
}