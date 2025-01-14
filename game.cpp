#include <iostream>
#include "game.h"
#include "logic.h"

using std::cin;
using std::cout;
using std::endl;

const size_t INPUT_ARRAY_SIZE = 32;
const char SEPARATOR = ' ';

struct GameInfo
{
	bool isGameOver;
	bool player;
	size_t boardSize;
	Board board;
	size_t AttackersScore;
	size_t DeffendersScore;
	HistoryStack history;
};

size_t countSeparators(char* const string, char separator)
{
	size_t count = 0;
	char* tmp = string;
	while (*tmp)
	{
		if (*tmp == separator)
			count++;

		tmp++;
	}

	return count;
}

void fillSplitArr(char* const string, char separator, char** splitArr)
{
	char* tmp = string;
	size_t splitSize = 0, currentIndex = 0;
	while (*tmp)
	{
		if (*tmp == separator)
		{
			splitArr[currentIndex] = new char[splitSize + 1];
			splitSize = 0;
			currentIndex++;
			tmp++;
		}
		splitSize++;
		tmp++;
	}
}

char** splitStr(char* const string, char separator)
{
	if (string == nullptr)
		return nullptr;

	char** splitArr = new char* [countSeparators(string, separator)];
	fillSplitArr(string, separator, splitArr);

	char* tmp = string;
	size_t currentSplitIndex = 0, currentIndex = 0;
	while (*tmp)
	{
		if (*tmp == separator)
		{
			splitArr[currentSplitIndex][currentIndex] = '\0';

			currentIndex = 0;
			currentSplitIndex++;
			tmp++;
		}

		splitArr[currentSplitIndex][currentIndex] = *tmp;
		currentIndex++;
		tmp++;
	}

	return splitArr;
}

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

uint8_t charToValue(char digit)
{
	return digit - '0';
}

size_t stringToValue(char* const string)
{
	size_t value = 0;
	char* tmp = string;
	while (*tmp)
	{
		value = value * 10 + charToValue(*tmp);
	}
	return value;
}

bool moveCommand(GameInfo* gameInfo, char** args)
{
	Position piece{};
	piece.x = stringToValue(args[0]);
	piece.y = stringToValue(args[1]);

	Position move{};
	move.x = stringToValue(args[2]);
	move.y = stringToValue(args[3]);

	return moveOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, &piece, &move, gameInfo->isGameOver);
}

bool backCommand(GameInfo* gameInfo)
{
	if (!gameInfo->player)
		return backOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, gameInfo->player, gameInfo->AttackersScore);
	else
		return backOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, gameInfo->player, gameInfo->DeffendersScore);
}

void infoCommand(GameInfo* gameInfo)
{

}

void game()
{
	char input[INPUT_ARRAY_SIZE]{};
	char** split;
	while (true)
	{
		printBoard()

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, SEPARATOR);
	}
}

void run()
{
	char input[INPUT_ARRAY_SIZE]{};
	while (true)
	{
		cin.getline(input, INPUT_ARRAY_SIZE);

	}
}