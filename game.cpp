#include <iostream>
#include "game.h"
#include "logic.h"

using std::cin;
using std::cout;
using std::endl;

const size_t INPUT_ARRAY_SIZE = 32;
const char SEPARATOR = ' ';

const char* MENU_MESSAGE = "1) NewGame <board type>\nBoard types: 9x9, 11x11, 13x13\n2) Quit\n";
const char* GAME_MESSAGE = "1) Move <piece position> <move position>\n2) Back\n3) Info\n4) ";

// Command strings
const char* NEW_GAME = "NewGame";
const char* NEW_GAME_SMALL = "newgame";
const char* QUIT = "Quit";
const char* QUIT_SMALL = "quit";

const char* MOVE = "Move";
const char* MOVE_SMALL = "move";
const char* BACK = "Back";
const char* BACK_SMALL = "back";
const char* INFO = "Info";
const char* INFO_SMALL = "info";

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

void setGameInfo(GameInfo* gameInfo, size_t boardSize)
{
	gameInfo->isGameOver = false;
	gameInfo->player = 0;
	if (boardSize > 0)
	{
		newBoard(gameInfo->board, gameInfo->boardSize, boardSize);
		gameInfo->boardSize = boardSize;
	}
	else
	{
		gameInfo->board = nullptr;
		gameInfo->boardSize = 0;
	}
	gameInfo->boardSize = 0;
	gameInfo->AttackersScore = 0;
	gameInfo->DeffendersScore = 0;
	gameInfo->history = nullptr;
}

GameInfo* allocateGameInfoMemory()
{
	GameInfo* gameInfo = new GameInfo;

	setGameInfo(gameInfo, 0);

	return gameInfo;
}

void deallocateGameInfoMemory(GameInfo* gameInfo)
{
	delete &gameInfo->isGameOver;
	delete &gameInfo->player;
	closeBoard(gameInfo->board, gameInfo->boardSize);
	delete &gameInfo->boardSize;
	delete &gameInfo->AttackersScore;
	delete &gameInfo->DeffendersScore;
	deallocateHistoryStackMemory(gameInfo->history);
	delete gameInfo;
}

size_t countSeparators(const char* string, char separator)
{
	size_t count = 0;
	while (*string)
	{
		if (*string == separator)
			count++;

		string++;
	}

	return count;
}

void fillSplitArr(const char* string, char separator, char** splitArr)
{
	size_t splitSize = 0, currentIndex = 0;
	while (*string)
	{
		if (*string == separator)
		{
			splitArr[currentIndex] = new char[splitSize + 1];
			splitSize = 0;
			currentIndex++;
			string++;
		}
		splitSize++;
		string++;
	}
}

char** splitStr(const char* string, char separator)
{
	if (string == nullptr)
		return nullptr;

	char** splitArr = new char* [countSeparators(string, separator)];
	fillSplitArr(string, separator, splitArr);

	size_t currentSplitIndex = 0, currentIndex = 0;
	while (*string)
	{
		if (*string == separator)
		{
			splitArr[currentSplitIndex][currentIndex] = '\0';

			currentIndex = 0;
			currentSplitIndex++;
			string++;
		}

		splitArr[currentSplitIndex][currentIndex] = *string;
		currentIndex++;
		string++;
	}

	return splitArr;
}

size_t stringLength(const char* string)
{
	if (string == nullptr)
		return 0;

	size_t length = 0;
	while (*string)
	{
		length++;
	}
	return length;
}

bool compareString(const char* leftString, const char* rightString)
{
	if (leftString == rightString)
		return true;

	size_t leftLength = stringLength(leftString),
		rightLength = stringLength(rightString);

	if (leftLength == rightLength)
	{
		while (*leftString)
		{
			if (*leftString != *rightString)
				return false;
		}
		return true;
	}
	return false;
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

bool newGame(GameInfo* gameInfo, char** split)
{
	
}

void game(GameInfo* gameInfo, char input[INPUT_ARRAY_SIZE], char** split)
{
	while (true)
	{
		printBoard(gameInfo->board, gameInfo->boardSize);

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, SEPARATOR);
	}
}

void run()
{
	GameInfo* gameInfo = allocateGameInfoMemory();

	char input[INPUT_ARRAY_SIZE]{};
	char** split;
	while (true)
	{

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, SEPARATOR);
		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
			break;
		else if (compareString(split[0], NEW_GAME) || compareString(split[0], NEW_GAME_SMALL))
	}
	deallocateGameInfoMemory(gameInfo);
}