#pragma once

struct Position;

// History stack node
struct MoveNode
{
	// An array of positions of taken pieces
	size_t takenSize;
	Position* taken;

	// Previous piece position
	Position* piece;

	// Move position
	Position* move;

	// Previous move
	MoveNode* previous;
};

typedef MoveNode* HistoryStack;

void deleteMoveNode(MoveNode* node);

void deallocateHistoryStackMemory(HistoryStack& history);

void saveMove(HistoryStack& history, Position* piece, Position* move);

void saveMove(HistoryStack& history, Position* piece, Position* move, Position* taken, size_t takenSize);

MoveNode* backMove(HistoryStack& history);