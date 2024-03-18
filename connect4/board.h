#pragma once
#include <stdio.h>

#define EMPTY 0
#define PLAYER 1
#define AI 2
#define PLAYER_SIGN 'X'
#define AI_SIGN 'o'

#define ROW_COUNT 6
#define COLUMN_COUNT 7

#define true 1
#define false 0

void resetBoard(int board[ROW_COUNT][COLUMN_COUNT]);
void printBoard(int board[ROW_COUNT][COLUMN_COUNT]);
int isValidColumn(int board[ROW_COUNT][COLUMN_COUNT], int column);
void dropPiece(int board[ROW_COUNT][COLUMN_COUNT], int column, int piece);
int winningWindow(int window[4], int piece);
int winningMove(int board[ROW_COUNT][COLUMN_COUNT], int piece);
int isDraw(int board[ROW_COUNT][COLUMN_COUNT]);
int* getValidLocations(int board[ROW_COUNT][COLUMN_COUNT]);
