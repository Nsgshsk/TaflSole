#pragma once

enum BoardSizes
{
	SMALL = 9,
	MEDIUM = 11,
	BIG = 13
};

enum BoardCellTypes
{
	EMPTY_SPACE = '-',
	ATTACKER = 'A',
	DEFENDER = 'D',
	KING = 'K',
	END_POINT = 'X'
};