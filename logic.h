#pragma once

struct Position
{
	size_t x, y;
};

struct MoveNode;

typedef MoveNode* HistoryStack;
typedef char** Board;

bool newBoard(Board& board, size_t oldSize, size_t newSize);

void closeBoard(Board board, size_t size);

void deallocateHistoryStackMemory(HistoryStack& history);

bool moveOperation(HistoryStack& history, Board board, size_t size, Position* piece, Position* move, bool& isGameOver);

bool backOperation(HistoryStack& history, Board board, size_t size, bool player, size_t& playerScore);