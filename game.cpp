#include <iostream>
#include "game.h"
#include "logic.h"

using std::cin;
using std::cout;
using std::endl;

const uint8_t LETTER_CHANGE = 32;

const size_t INPUT_ARRAY_SIZE = 32;
const char ARGUMENT_SEPARATOR = ' ';
const char TYPE_SEPARATOR = 'x';

const size_t MOVE_ARGS = 4;

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
	splitArr[currentIndex + 1] = new char;
}

char** splitStr(const char* string, char separator)
{
	if (string == nullptr)
		return nullptr;

	char** splitArr = new char* [countSeparators(string, separator) + 1 + 1];
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
	*splitArr[currentSplitIndex + 1] = -1;

	return splitArr;
}

size_t splitArrLength(char** split)
{
	size_t length = 0;
	while (**split != -1)
	{
		length++;
		split++;
	}
	return length;
}

void deleteSplitString(char** split)
{
	char** tmp = split;
	while (**tmp != -1)
	{
		delete[] *tmp;
		tmp++;
	}
	delete[] split;
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

uint8_t digitToValue(char digit)
{
	return digit - '0';
}

uint8_t letterToValue(char letter)
{
	if ('a' <= letter && letter <= 'z')
		letter ^= LETTER_CHANGE;

	return letter - 'A';
}

uint8_t charToValue(char ch)
{
	if ('0' <= ch && ch <= '9')
		return digitToValue(ch);
	else
		return letterToValue(ch);
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

bool coordinatesToValues(size_t boardSize, char** coordinates, size_t valuesArr[MOVE_ARGS])
{
	// Translates first set of coordinates
	valuesArr[0] = letterToValue(coordinates[0][0]); // Gets coordinate from letter
	valuesArr[1] = boardSize - stringToValue(coordinates[0] + 1); // Gets coordinate from num after letter

	// Translates second set of coordinates
	valuesArr[2] = letterToValue(coordinates[1][0]); // Gets coordinate from letter
	valuesArr[3] = boardSize - stringToValue(coordinates[1] + 1); // Gets coordinate from num after letter

	return true;
}

bool moveCommand(GameInfo* gameInfo, char** args)
{
	size_t coordinateValuesArr[MOVE_ARGS]{};
	if (!coordinatesToValues(gameInfo->boardSize, args, coordinateValuesArr))
		return false;

	Position* piece = createPosition(coordinateValuesArr[1], coordinateValuesArr[0]);
	Position* move = createPosition(coordinateValuesArr[3], coordinateValuesArr[2]);

	bool result = moveOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, piece, move, gameInfo->isGameOver, gameInfo->player);

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

bool newGame(GameInfo* gameInfo, char* arg)
{
	size_t newSize = stringToValue(arg);
	bool success = newBoard(gameInfo->board, gameInfo->boardSize, newSize);
	if (success)
		gameInfo->boardSize = newSize;
	return success;
}

void changePlayer(bool& player)
{
	player = !player;
}

bool game(GameInfo* gameInfo, char input[INPUT_ARRAY_SIZE], char** split)
{
	char** args = splitStr(split[1], TYPE_SEPARATOR);
	if (!compareString(args[0], args[1]))
		return false;

	if (!newGame(gameInfo, args[0]))
		return false;

	while (!gameInfo->isGameOver)
	{
		printBoard(gameInfo->board, gameInfo->boardSize);
		cout << (!gameInfo->player ? "Attackers" : "Defenders") << " turn: ";

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, ARGUMENT_SEPARATOR);

		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
		{
			break;
		}
		else if (compareString(split[0], MOVE) || compareString(split[0], MOVE_SMALL))
		{
			if (splitArrLength(split) == MOVE_ARGS - 1 && moveCommand(gameInfo, split + 1))
				changePlayer(gameInfo->player);
		}
		else if (compareString(split[0], BACK) || compareString(split[0], BACK_SMALL))
		{
			if (backCommand(gameInfo))
				changePlayer(gameInfo->player);
		}
		else if (compareString(split[0], INFO) || compareString(split[0], INFO_SMALL))
			infoCommand(gameInfo);
		else
			cout << "Command isn't recognized!";

		cout << endl;
		deleteSplitString(split);
	}

	cout << !gameInfo->player << " wins";

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
			if (splitArrLength(split) == 2 && !game(gameInfo, input, split))
				cout << "Type doesn't exist!";
		}
		else
			cout << "Command isn't recognized!";

		cout << endl;
		deleteSplitString(split);
	}
	deleteSplitString(split);
	deallocateGameInfoMemory(gameInfo);
}