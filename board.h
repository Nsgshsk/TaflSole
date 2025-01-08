#pragma once

typedef char** Board;

bool newBoard(Board& board, size_t oldSize, size_t newSize);

void closeBoard(Board board, size_t size);

bool changeCell(Board board, size_t size, size_t row, size_t col, char cellType);

char typeOfCell(Board const board, size_t size, size_t row, size_t col);