#pragma once

typedef char** Board;

struct GameInfo
{
	Board board;
	size_t AttackersScore;
	size_t DeffendersScore;

};