/*
sources:
    https://en.wikipedia.org/wiki/Minimax
    https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>
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

int randInt(int low, int high) {
    srand(time(NULL));
    int random_number = low + rand() % (high - low + 1);
    return random_number;
}

int randChoice(int validLocations[COLUMN_COUNT]) {
    int validColumns = 0;

    for(int i=0; i<COLUMN_COUNT; i++) {
        if(validLocations[i])
            validColumns += 1;
    }

    int selectedColumn = randInt(1, validColumns);
    int index = 0;

    while(true) {
        if(validLocations[index]) {
            selectedColumn--;
        }

        if(selectedColumn == 0) {
            return index;
        }

        index++;
    }
}

int getNextOpenRow(int board[ROW_COUNT][COLUMN_COUNT], int col) {
    for(int r=0; r<ROW_COUNT; r++) {
        if(board[r][col] == EMPTY)
            return r;
    }
}

void copyBoard(int board[ROW_COUNT][COLUMN_COUNT], int boardCopied[ROW_COUNT][COLUMN_COUNT]) {
    for(int r=0; r<ROW_COUNT; r++) {
        for(int c=0; c<COLUMN_COUNT; c++) {
            boardCopied[r][c] = board[r][c];
        }
    }
}

int oppositPiece(int player) {
    if(player == PLAYER) return AI;
    else return PLAYER;
}

double windowEvaluate(int window[4], int piece) {
    double score = 0;
    int oppPiece = oppositPiece(piece);
    
    int countPiece = 0;
    int countOppPiece = 0;
    int countEmpty = 0;

    for(int i=0; i<4; i++) {
        if(window[i] == EMPTY) countEmpty += 1;
        else if(window[i] == piece) countPiece += 1;
        else if(window[i] == oppPiece) countOppPiece += 1;
    }

    if(countPiece == 3 && countEmpty == 1) score += 5.0;
    if(countPiece == 2 && countEmpty == 2) score += 2.0;
    if(countOppPiece == 3 && countEmpty == 1) score -= 4.0;

    return score;
}

double scorePosition(int board[ROW_COUNT][COLUMN_COUNT], int piece) {
    double score = 0;

    // center column
    for(int r=0; r<ROW_COUNT; r++) {
        if(board[r][COLUMN_COUNT/2] == piece) score += 3.0;
    }

    int window[4];

    // horizontal
    for(int r=0; r<ROW_COUNT; r++) {
        for(int c=0; c<COLUMN_COUNT-3; c++) {
            for(int i=0; i<4; i++) window[i] = board[r][c+i];
            score += windowEvaluate(window, piece);
        }
    }

    // vertivcal
    for(int r=0; r<ROW_COUNT-3; r++) {
        for(int c=0; c<COLUMN_COUNT; c++) {
            for(int i=0; i<4; i++) window[i] = board[r+i][c];
            score += windowEvaluate(window, piece);
        }
    }

    // diagonal
    for(int r=0; r<ROW_COUNT-3; r++) {
        for(int c=0; c<COLUMN_COUNT-3; c++) {
            // diagonal left
            for(int i=0; i<4; i++) window[i] = board[r+i][c+i];
            score += windowEvaluate(window, piece);
            // diagonal right
            for(int i=0; i<4; i++) window[i] = board[r+i][c+3-i];
            score += windowEvaluate(window, piece);
        }
    }

    return score;
}

int getColFromScore(double scores[COLUMN_COUNT], double targetValue) {
    int columns[COLUMN_COUNT] = {false};
    for(int i=0; i<COLUMN_COUNT; i++) {
        if(scores[i] == targetValue) columns[i] = true;
    }
    return randChoice(columns);
}

int isTerminalNode(int board[ROW_COUNT][COLUMN_COUNT]) {
    if(
        winningMove(board, PLAYER) ||
        winningMove(board, AI) ||
        isDraw(board)
    ) return true;
    return false;
}

double* minimax(int board[ROW_COUNT][COLUMN_COUNT], int depth, double alpha, double beta, int maximizingPlayer) {
    int* validLocations = getValidLocations(board);
    int isTerminal = isTerminalNode(board);
    static double result[2] = {-1.0, 0.0};

    if(depth == 0 || isTerminal) {
        if(isTerminal) {
            if(winningMove(board, AI)) {
                result[1] = 100000.0;
                return result;
            }
            else if(winningMove(board, PLAYER)) {
                result[1] = -100000.0;
                return result;
            }
            else
                return result;
        }
        else {
            result[1] = scorePosition(board, AI);
            return result;
        }
    }

    if(maximizingPlayer) {
        double scores[COLUMN_COUNT] = {NAN};
        double value = LOW_VALUE;
        double column;
        column = (double)randChoice(validLocations);

        for(int c=0; c<COLUMN_COUNT; c++) {
            if(validLocations[c]) {
                int row = getNextOpenRow(board, c);
                int boardCopied[ROW_COUNT][COLUMN_COUNT];
                copyBoard(board, boardCopied);
                dropPiece(boardCopied, c, AI);

                double newScore = minimax(boardCopied, depth-1, alpha, beta, false)[1] / 2;

                scores[c] = newScore;

                if(newScore > value) {
                    value = newScore;
                    column = (double)c;
                }

                if(value > alpha) alpha = value;
                if(beta <= alpha) break;
            }
        }

        result[0] = getColFromScore(scores, value);
        result[1] = value;

        return result;
    }
    else {
        double scores[COLUMN_COUNT] = {NAN};
        double value = HIGH_VALUE;
        double column;
        column = (double)randChoice(validLocations);

        for(int c=0; c<COLUMN_COUNT; c++) {
            if(validLocations[c]) {
                int row = getNextOpenRow(board, c);
                int boardCopied[ROW_COUNT][COLUMN_COUNT];
                copyBoard(board, boardCopied);
                dropPiece(boardCopied, c, PLAYER);

                double newScore = minimax(boardCopied, depth-1, alpha, beta, true)[1] / 2;

                scores[c] = newScore;

                if(newScore < value) {
                    value = newScore;
                    column = (double)c;
                }

                if(value < beta) beta = value;
                if(beta <= alpha) break;
            }
        }

        result[0] = getColFromScore(scores, value);
        result[1] = value;

        return result;
    }
}
