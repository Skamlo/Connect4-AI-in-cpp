#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0
#define PLAYER 1
#define AI 2
#define PLAYER_SIGN 'X'
#define AI_SIGN 'o'

#define ROW_COUNT 6
#define COLUMN_COUNT 7

#define true 1
#define false 0

int randInt(int low, int high) {
    srand(time(NULL));
    int random_number = low + rand() % (high - low + 1);
    return random_number;
}

void resetBoard(int board[ROW_COUNT][COLUMN_COUNT]) {
    for (int i = 0; i < ROW_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            board[i][j] = 0;
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
    if(board[ROW_COUNT-1][column-1] == 0) {
        return true;
    }
    else {
        return false;
    }
}

void dropPiece(int board[ROW_COUNT][COLUMN_COUNT], int column, int piece) {
    if(isValidColumn(board, column)) {
        for(int i=0; i<ROW_COUNT; i++) {
            if(board[i][column-1] == 0) {
                board[i][column-1] = piece;
                break;
            }
        }
    }
}

int changeTurn(int turn) {
    if(turn == 1) {
        return 2;
    }
    else {
        return 1;
    }
}

int getPlayerColumn() {
    printf("Enter column number: ");
    int col;
    while(true) {
        scanf("%d", &col);
        if(col >= 1 && col <= 7) {
            break;
        }
        printf("Invalid column number. Enter new: ");
    }
}

int main() {
    int board[ROW_COUNT][COLUMN_COUNT] = {0};
    printBoard(board);

    int turn = 1;

    while(true) {
        if(turn == 1) {
            int col = getPlayerColumn();
            dropPiece(board, col, PLAYER);
            printBoard(board);

            turn = changeTurn(turn);
        }
        else {
            int col = randInt(1, 7);
            dropPiece(board, col, AI);
            printBoard(board);

            turn = changeTurn(turn);
        }
    }
    
}
