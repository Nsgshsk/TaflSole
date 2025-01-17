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
* Contains three commonly reused position checking functions
*
*/

#include "positionChecks.h"

bool isOutOfBounds(size_t boardSize, size_t row, size_t col)
{
	if (row < 0 || row >= boardSize)
		return true;

	else if (col < 0 || col >= boardSize)
		return true;

	return false;
}

bool isCorner(size_t boardSize, size_t row, size_t col)
{
	if (row == 0)
		return col == 0 || col == boardSize - 1;

	else if (row == boardSize - 1)
		return col == 0 || col == boardSize - 1;

	return false;
}

bool isKingStartingPosition(size_t boardSize, size_t row, size_t col)
{
	size_t middle = boardSize / 2;
	return row == middle && col == middle;
}