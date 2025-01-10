#pragma once

struct MoveNode;

typedef MoveNode* HistoryStack;

typedef char** Board;

struct GameInfo
{
	Board board;
	size_t AttackersScore;
	size_t DeffendersScore;
	HistoryStack history;
};