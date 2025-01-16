#include "history.h"

MoveNode* createMoveNode(Position* piece, Position* move)
{
	MoveNode* newNode = new MoveNode;

	// Node data initialization
	newNode->takenSize = 0;
	newNode->taken = nullptr;
	newNode->piece = piece;
	newNode->move = move;

	return newNode;
}

// Overload of createMoveNode function
MoveNode* createMoveNode(Position* piece, Position* move, Position* taken, size_t takenSize)
{
	MoveNode* newNode = createMoveNode(piece, move);

	// Initializing taken array data
	newNode->takenSize = takenSize;
	newNode->taken = taken;

	return newNode;
}

// Frees memory space taken by MoveNode
void deleteMoveNode(MoveNode* node)
{
	if (node->taken != nullptr)
		delete[] node->taken;
	delete node->piece;
	delete node->move;
	delete node;
}

// Standard push stack function - adds new move to history
void push(HistoryStack& history, MoveNode* move)
{
	move->previous = history;
	history = move;
}

// Standard pop stack function - returns a pointer to the backed move
MoveNode* pop(HistoryStack& history)
{
	if (history == nullptr)
		return nullptr;

	HistoryStack backedMove = history;
	history = backedMove->previous;

	return backedMove;
}

// Wrapper function of push with creation of a move
void saveMove(HistoryStack& history, Position* piece, Position* move)
{
	MoveNode* current = createMoveNode(piece, move);
	push(history, current);
}

// Overload of saveMove function 
void saveMove(HistoryStack& history, Position* piece, Position* move, Position* taken, size_t takenSize)
{
	MoveNode* current = createMoveNode(piece, move, taken, takenSize);
	push(history, current);
}

// Wrapper function of pop
MoveNode* backMove(HistoryStack& history)
{
	return pop(history);
}

// Deallocates all memory from HistoryStack
void deallocateHistoryStackMemory(HistoryStack& history)
{
	while (history != nullptr)
	{
		deleteMoveNode(pop(history));
	}
}