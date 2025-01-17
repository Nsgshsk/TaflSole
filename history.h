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
* Header file for history.cpp that defines Position and MoveNode structs
*
*/

#pragma once

struct Position
{
	size_t x, y;
};

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