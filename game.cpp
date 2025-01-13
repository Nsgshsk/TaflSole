#include <iostream>
#include "game.h"
#include "logic.h"

using std::cin;
using std::cout;
using std::endl;

struct Position
{
	size_t x, y;
};

struct GameInfo
{
	bool isGameOver;
	size_t boardSize;
	Board board;
	size_t AttackersScore;
	size_t DeffendersScore;
	HistoryStack history;
};

void printBoard(const Board board, size_t size)
{
	for (size_t row = 0; row < size; row++)
	{
		for (size_t col = 0; col < size; col++)
		{
			cout << board[row][col] << ' ';
		}
		cout << "|" << size - row;
		cout << endl;
	}
	for (size_t i = 0; i < (2 * size + 1); i++)
	{
		cout << '_';
	}
	cout << endl;
	for (char ch = 'a'; ch < ('a' + size); ch++)
	{
		cout << ch << ' ';
	}
	cout << endl;
}

// in progress
bool moveCommand(GameInfo* gameInfo)
{
	Position piece{};
	cin >> piece.x >> piece.y;

	Position move{};
	cin >> move.x >> move.y;

	while (!moveOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, &piece, &move, gameInfo->isGameOver))
	{
		cin >> piece.x >> piece.y;
		cin >> move.x >> move.y;
	}
}