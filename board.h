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
* Header file for board.cpp
*
*/

#pragma once

typedef char** Board;

bool newBoard(Board& board, size_t oldSize, size_t newSize);

void closeBoard(Board board, size_t size);

bool changeCell(Board board, size_t size, size_t row, size_t col, char cellType);

char typeOfCell(const Board board, size_t size, size_t row, size_t col);