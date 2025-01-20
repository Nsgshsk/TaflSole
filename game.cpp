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

#include "constants.h"
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

// Message strings
const char* MENU_MESSAGE = "Menu\n-------\n ) Newgame <board type>\nBoard types: 9x9, 11x11, 13x13\n ) Quit\n";
const char* HELP_MESSAGE = "Help\n-------\n ) Move <from_coordinates> <to_coordinates>\n ) Back\n ) Info\n ) Quit";
const char* CONFIRMATION_MESSAGE = "You will lose the game by quiting!\nAre you sure? (Y\\N) ";
const char* PROMT_MESSAGE = "> ";
const char* COMMAND_NOT_RECOGNIZED = "Command isn't recognized!";
const char* TYPE_NOT_RECOGNIZED = "Type isn't recognized!";
const char* ERROR_MESSAGE = "Invalid command use!";
const char* HELP_REMINDER_MESSAGE = "If you are not sure how to use a command use \"help\"";
const char* CANCEL_MESSAGE = "Quit canceled...";

// Command strings
const char* NEW_GAME = "Newgame";
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

// Stores all info about the game
struct GameInfo
{
	bool isGameOver;
	bool player;
	size_t turn;
	size_t boardSize;
	Board board;
	size_t AttackersScore;
	size_t DefendersScore;
	HistoryStack history;
};

// Function sets default values for GameInfo struct
bool setGameInfo(GameInfo* gameInfo, size_t boardSize)
{
	gameInfo->isGameOver = false;
	gameInfo->player = 0;
	gameInfo->turn = 0;
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
	gameInfo->DefendersScore = 0;
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

// String helper functions

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

// Allocates memory for split array
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

// Standard splitStr function with marked end of array (-1)
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

// Functions for commands

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
			gameInfo->isGameOver, gameInfo->player, gameInfo->DefendersScore);

	return result;
}

bool backCommand(GameInfo* gameInfo)
{
	if (gameInfo->player)
		return backOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, gameInfo->player, gameInfo->AttackersScore);
	else
		return backOperation(gameInfo->history, gameInfo->board, gameInfo->boardSize, gameInfo->player, gameInfo->DefendersScore);
}

void infoCommand(GameInfo* gameInfo)
{
	// Gets needed piece count depending on board type
	size_t pieces = 0, attackerPieces = 0, defenderPieces = 0;
	switch (gameInfo->boardSize)
	{
		case SMALL:
			pieces = SMALL_ATTACKER + SMALL_DEFENDER;
			attackerPieces = SMALL_ATTACKER - gameInfo->DefendersScore;
			defenderPieces = SMALL_DEFENDER - gameInfo->AttackersScore;
			break;
		case MEDIUM:
			pieces = MEDIUM_ATTACKER + MEDIUM_DEFENDER;
			attackerPieces = MEDIUM_ATTACKER - gameInfo->DefendersScore;
			defenderPieces = MEDIUM_DEFENDER - gameInfo->AttackersScore;
			break;
		case BIG:
			pieces = BIG_ATTACKER + BIG_DEFENDER;
			attackerPieces = BIG_ATTACKER - gameInfo->DefendersScore;
			defenderPieces = BIG_DEFENDER - gameInfo->AttackersScore;
			break;
		default:
			cout << "Achievement unlocked: How did we get here? :O\n";
			return;
	}

	pieces -= gameInfo->AttackersScore + gameInfo->DefendersScore;

	cout << "Game Info" << endl
		<< "----------" << endl
		<< "Turn: " << gameInfo->turn << endl
		<< endl
		<< "Attackers took: " << gameInfo->AttackersScore << endl
		<< "Defenders took: " << gameInfo->DefendersScore << endl
		<< endl
		<< "Left pieces: " << pieces << endl
		<< "Attacker pieces left: " << attackerPieces << endl
		<< "Defender pieces left: " << defenderPieces << endl;
}

// Helper function for printing board
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

// Resets GameInfo for new game
bool newGame(GameInfo* gameInfo, char* arg)
{
	size_t newSize = stringToValue(arg);
	return setGameInfo(gameInfo, newSize);
}

// Helper function
void changePlayer(bool& player)
{
	player = !player;
}

// Clears console using ANSI escape codes, it may not be supported by some consoles or OS-es
void clear()
{
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\x1B[2J\x1B[H";
}

// Main game loop
bool game(GameInfo* gameInfo, char input[INPUT_ARRAY_SIZE], char** split)
{
	char** args = splitStr(split[1], TYPE_SEPARATOR);
	if (!compareString(args[0], args[1]))
		return false;

	if (!newGame(gameInfo, args[0]))
		return false;

	uint8_t invalidTries = 0;
	while (!gameInfo->isGameOver)
	{
		printBoard(gameInfo->board, gameInfo->boardSize);
		cout << endl << (!gameInfo->player ? "Attackers" : "Defenders") << " turn" << PROMT_MESSAGE;

		cin.getline(input, INPUT_ARRAY_SIZE);
		split = splitStr(input, ARGUMENT_SEPARATOR);

		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
		{
			cout << endl << CONFIRMATION_MESSAGE;
			cin.getline(input, INPUT_ARRAY_SIZE);
			if (compareString(input, "Y") || compareString(input, "y"))
				break;
			clear();
			cout << CANCEL_MESSAGE;
		}
		else if (clear(), compareString(split[0], MOVE) || compareString(split[0], MOVE_SMALL))
		{
			if (splitArrLength(split) == MOVE_ARGS - 1 && moveCommand(gameInfo, split + 1))
			{
				changePlayer(gameInfo->player);
				
				// Checks one of the game ending conditions but chose not to use it
				/*if (player && isGameOverCondition(gameInfo->DefendersScore))
					break;*/

				gameInfo->turn++;

				deleteSplitString(split);
				continue;
			}
			else
			{
				cout << ERROR_MESSAGE;
				invalidTries++;
			}
		}
		else if (compareString(split[0], BACK) || compareString(split[0], BACK_SMALL))
		{
			if (backCommand(gameInfo))
			{
				changePlayer(gameInfo->player);
				gameInfo->turn--;

				deleteSplitString(split);
				continue;
			}
			else
			{
				cout << ERROR_MESSAGE;
				invalidTries++;
			}
		}
		else if (compareString(split[0], INFO) || compareString(split[0], INFO_SMALL))
			infoCommand(gameInfo);
		else if (compareString(split[0], HELP) || compareString(split[0], HELP_SMALL))
		{
			cout << HELP_MESSAGE;
			invalidTries = 0;
		}
		else
		{
			cout << COMMAND_NOT_RECOGNIZED;
			invalidTries++;
		}

		if (invalidTries == 5)
		{
			cout << endl << HELP_REMINDER_MESSAGE;
			invalidTries = 0;
		}

		cout << endl << endl;
		deleteSplitString(split);
	}

	clear();
	printBoard(gameInfo->board, gameInfo->boardSize);
	cout << (!gameInfo->player ? "Defenders" : "Attackers") << " wins.\n";

	return true;
}

// Game entry point
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

		clear();

		if (compareString(split[0], QUIT) || compareString(split[0], QUIT_SMALL))
			break;
		else if (compareString(split[0], NEW_GAME) || compareString(split[0], NEW_GAME_SMALL))
		{
			if (splitArrLength(split) != 2)
				cout << ERROR_MESSAGE;
			else if (!game(gameInfo, input, split))
				cout << TYPE_NOT_RECOGNIZED;
		}
		else
			cout << COMMAND_NOT_RECOGNIZED;

		cout << endl << endl;
		deleteSplitString(split);
	}

	cout << "Closing game...";
	deleteSplitString(split);
	deallocateGameInfoMemory(gameInfo);
}
