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
* Handles user interface, input processing, game flow and main game loop
*
*/

#include "game.h"
#include "logic.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

const uint8_t LETTER_CHANGE = 32;

const size_t INPUT_ARRAY_SIZE = 32;
const char ARGUMENT_SEPARATOR = ' ';
const char TYPE_SEPARATOR = 'x';

const size_t MOVE_ARGS = 4;

const char* MENU_MESSAGE = "1) NewGame <board type>\nBoard types: 9x9, 11x11, 13x13\n2) Quit\n";
const char* HELP_MESSAGE = "1) Move <piece coordinates> <move coordinates>\n2) Back\n3) Info\n4) Quit";
const char* CONFIRMATION_MESSAGE = "Are you sure? (Y\\N)";
const char* PROMT_MESSAGE = "> ";

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
const char* HELP = "Help";
const char* HELP_SMALL = "help";

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

bool setGameInfo(GameInfo* gameInfo, size_t boardSize)
{
	gameInfo->isGameOver = false;
	gameInfo->player = 0;
	if (boardSize > 0)
	{
		if (!newBoard(gameInfo->board, gameInfo->boardSize, boardSize))
			return false;
		gameInfo->boardSize = boardSize;
	}
	else
	{
		gameInfo->board = nullptr;
		gameInfo->boardSize = 0;
	}
	gameInfo->AttackersScore = 0;
	gameInfo->DeffendersScore = 0;
	gameInfo->history = nullptr;

	return true;
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

	bool result;
	if (!gameInfo->player)
		result = moveOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, piece, move,
			gameInfo->isGameOver, gameInfo->player, gameInfo->AttackersScore);
	else
		result = moveOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, piece, move,
			gameInfo->isGameOver, gameInfo->player, gameInfo->DeffendersScore);
	return result;
}

bool backCommand(GameInfo* gameInfo)
{
	if (gameInfo->player)
		return backOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, gameInfo->player, gameInfo->AttackersScore);
	else
		return backOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, gameInfo->player, gameInfo->DeffendersScore);
}

void infoCommand(GameInfo* gameInfo)
{
	cout << "Game Info" << endl
		<< "Attackers score: " << gameInfo->AttackersScore << endl
		<< "Defenders score: " << gameInfo->DeffendersScore << endl;
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
	return setGameInfo(gameInfo, newSize);
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
		cout << (!gameInfo->player ? "Attackers" : "Defenders") << " turn" << PROMT_MESSAGE;

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, ARGUMENT_SEPARATOR);

		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
		{
			cout << CONFIRMATION_MESSAGE;
			cin.getline(input, INPUT_ARRAY_SIZE);
			if (compareString(input, "Y") || compareString(input, "y"))
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
		else if (compareString(split[0], HELP) || compareString(split[0], HELP_SMALL))
			cout << HELP_MESSAGE;
		else
			cout << "Command isn't recognized!";

		cout << endl << endl;
		deleteSplitString(split);
	}

	printBoard(gameInfo->board, gameInfo->boardSize);
	cout << (!gameInfo->player ? "Defenders" : "Attackers") << " wins.\n";

	return true;
}

void run()
{
	GameInfo* gameInfo = allocateGameInfoMemory();

	char input[INPUT_ARRAY_SIZE]{};
	char** split;
	while (true)
	{
		cout << MENU_MESSAGE << endl << PROMT_MESSAGE;
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
	cout << "Closing game...";
	deleteSplitString(split);
	deallocateGameInfoMemory(gameInfo);
}