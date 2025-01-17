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
* Header file for logic.cpp
*
*/

#pragma once

struct Position;

struct MoveNode;

typedef MoveNode* HistoryStack;
typedef char** Board;

Position* createPosition(size_t x, size_t y);

void deletePosition(Position* pos);

bool newBoard(Board& board, size_t oldSize, size_t newSize);

void closeBoard(Board board, size_t size);

void deallocateHistoryStackMemory(HistoryStack& history);

bool moveOperation(HistoryStack& history, Board board, size_t size, Position* piece, Position* move, bool& isGameOver, bool player, size_t& playerScore);

bool backOperation(HistoryStack& history, Board board, size_t size, bool player, size_t& playerScore);