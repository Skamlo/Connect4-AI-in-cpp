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

void resetBoard(int board[ROW_COUNT][COLUMN_COUNT]) {
    for (int i = 0; i < ROW_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            board[i][j] = EMPTY;
        }
    }
}

void printBoard(int board[ROW_COUNT][COLUMN_COUNT]) {
    printf("  ");
    for(int c=0; c<COLUMN_COUNT; c++) {
        printf("%d   ", c+1);
    }
    printf("\n");

    for(int r=ROW_COUNT-1; r>-1; r--) {
        printf("|");
        for(int c=0; c<COLUMN_COUNT; c++) {
            switch (board[r][c]) {
            case 0:
                printf("   |");
                break;
            case 1:
                printf(" %c |", PLAYER_SIGN);
                break;
            case 2:
                printf(" %c |", AI_SIGN);
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int isValidColumn(int board[ROW_COUNT][COLUMN_COUNT], int column) {
    if(board[ROW_COUNT-1][column-1] == EMPTY) {
        return true;
    }
    else {
        return false;
    }
}

void dropPiece(int board[ROW_COUNT][COLUMN_COUNT], int column, int piece) {
    if(isValidColumn(board, column)) {
        for(int i=0; i<ROW_COUNT; i++) {
            if(board[i][column-1] == EMPTY) {
                board[i][column-1] = piece;
                break;
            }
        }
    }
}

int winningWindow(int window[4], int piece) {
    for(int i=0; i<4; i++) {
        if(window[i] != piece) {
            return false;
        }
    }
    return true;
}

int winningMove(int board[ROW_COUNT][COLUMN_COUNT], int piece) {
    int window[4];

    // horizontal
    for(int r=0; r<ROW_COUNT; r++) {
        for(int c=0; c<COLUMN_COUNT-3; c++) {
            for(int i=0; i<4; i++) window[i] = board[r][c+i];
            if(winningWindow(window, piece)) return true;
        }
    }

    // vertivcal
    for(int r=0; r<ROW_COUNT-3; r++) {
        for(int c=0; c<COLUMN_COUNT; c++) {
            for(int i=0; i<4; i++) window[i] = board[r+i][c];
            if(winningWindow(window, piece)) return true;
        }
    }

    // diagonal
    for(int r=0; r<ROW_COUNT-3; r++) {
        for(int c=0; c<COLUMN_COUNT-3; c++) {
            // diagonal left
            for(int i=0; i<4; i++) window[i] = board[r+i][c+i];
            if(winningWindow(window, piece)) return true;
            // diagonal right
            for(int i=0; i<4; i++) window[i] = board[r+i][c+3-i];
            if(winningWindow(window, piece)) return true;
        }
    }
    return false;
}
