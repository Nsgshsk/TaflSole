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
* Header file for positionChecks.cpp
*
*/

#pragma once

bool isOutOfBounds(size_t boardSize, size_t row, size_t col);

bool isCorner(size_t boardSize, size_t row, size_t col);

bool isKingStartingPosition(size_t boardSize, size_t row, size_t col);