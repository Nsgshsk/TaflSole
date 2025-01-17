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
* Contains reused constants
*
*/

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