#pragma once

typedef char** Board;

bool newBoard(Board& board, size_t oldSize, size_t newSize);

void closeBoard(Board board, size_t size);