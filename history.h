#pragma once

struct Position
{
	size_t x, y;
};

struct MoveNode;

typedef MoveNode* HistoryStack;

void deleteMoveNode(MoveNode* node);

void saveMove(HistoryStack& history, Position* piece, Position* move);

void saveMove(HistoryStack& history, Position* piece, Position* move, Position* taken, size_t takenSize);

MoveNode* backMove(HistoryStack& history);