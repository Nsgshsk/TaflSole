#pragma once

struct Position;

struct MoveNode;

typedef MoveNode* HistoryStack;

typedef char** Board;

bool moveOperation(HistoryStack& history, Board board, size_t size, Position* piece, Position* move, bool& isGameOver);

bool backOperation(HistoryStack& history, Board board, size_t size, bool player, size_t& playerScore);