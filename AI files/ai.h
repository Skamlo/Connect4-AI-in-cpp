#pragma once
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define EMPTY 0
#define PLAYER 1
#define AI 2

#define ROW_COUNT 6
#define COLUMN_COUNT 7

#define true 1
#define false 0

#define LOW_VALUE -1000000000
#define HIGH_VALUE 1000000000

int randInt(int low, int high);
int randChoice(int validLocations[COLUMN_COUNT]);
int getNextOpenRow(int board[ROW_COUNT][COLUMN_COUNT], int col);
int copyBoard(int board[ROW_COUNT][COLUMN_COUNT], int boardCopied[ROW_COUNT][COLUMN_COUNT]);
int oppositPiece(int player);
double windowEvaluate(int window[4], int piece);
double scorePosition(int board[ROW_COUNT][COLUMN_COUNT], int piece);
int isTerminalNode(int board[ROW_COUNT][COLUMN_COUNT]);
double* minimax(int board[ROW_COUNT][COLUMN_COUNT], int depth, double alpha, double beta, int maximizingPlayer);
