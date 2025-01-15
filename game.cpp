#include <iostream>
#include "game.h"
#include "logic.h"

using std::cin;
using std::cout;
using std::endl;

const size_t INPUT_ARRAY_SIZE = 32;
const char ARGUMENT_SEPARATOR = ' ';
const char TYPE_SEPARATOR = 'x';

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
	closeBoard(gameInfo->board, gameInfo->boardSize);
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
	splitArr[currentIndex] = new char[splitSize + 1];
}

char** splitStr(const char* string, char separator)
{
	if (string == nullptr)
		return nullptr;

	char** splitArr = new char* [countSeparators(string, separator) + 1];
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
	splitArr[currentSplitIndex][currentIndex] = '\0';

	return splitArr;
}

size_t stringLength(const char* string)
{
	if (string == nullptr)
		return 0;

	size_t length = 0;
	while (*string != '\0')
	{
		length++;
		string++;
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

			leftString++;
			rightString++;
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

size_t stringToValue(const char* string)
{
	size_t value = 0;
	while (*string)
	{
		value = value * 10 + charToValue(*string);
		string++;
	}
	return value;
}

bool moveCommand(GameInfo* gameInfo, char** args)
{
	Position* piece = createPosition(stringToValue(args[1]), stringToValue(args[2]));
	Position* move = createPosition(stringToValue(args[3]), stringToValue(args[4]));

	bool result = moveOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, piece, move, gameInfo->isGameOver);

	deletePosition(piece);
	deletePosition(move);

	return result;
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
	cout << "Not ready";
}

void newGame(GameInfo* gameInfo, char* arg)
{
	size_t newSize = stringToValue(arg);
	newBoard(gameInfo->board, gameInfo->boardSize, newSize);
	gameInfo->boardSize = newSize;
}

bool game(GameInfo* gameInfo, char input[INPUT_ARRAY_SIZE], char** split)
{
	char** args = splitStr(split[1], TYPE_SEPARATOR);
	if (!compareString(args[0], args[1]))
		return false;

	newGame(gameInfo, args[0]);

	while (true)
	{
		printBoard(gameInfo->board, gameInfo->boardSize);
		cout << (!gameInfo->player ? "Attackers" : "Defenders") << " turn: ";

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, ARGUMENT_SEPARATOR);

		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
		{
			if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
				break;
		}
		else if (compareString(split[0], MOVE) || compareString(split[0], MOVE_SMALL))
			moveCommand(gameInfo, split);
		else if (compareString(split[0], BACK) || compareString(split[0], BACK_SMALL))
			backCommand(gameInfo);
		else if (compareString(split[0], INFO) || compareString(split[0], INFO_SMALL))
			infoCommand(gameInfo);
		else
			cout << "Command isn't recognized!";

		cout << endl;
	}

	return true;
}

void run()
{
	GameInfo* gameInfo = allocateGameInfoMemory();

	char input[INPUT_ARRAY_SIZE]{};
	char** split;
	while (true)
	{
		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, ARGUMENT_SEPARATOR);
		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
			break;
		else if (compareString(split[0], NEW_GAME) || compareString(split[0], NEW_GAME_SMALL))
		{
			if (!game(gameInfo, input, split))
				cout << "Type doesn't exist!";
		}
		else
			cout << "Command isn't recognized!";

		cout << endl;
	}
	deallocateGameInfoMemory(gameInfo);
}