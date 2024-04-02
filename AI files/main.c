#include <stdio.h>
#include "board.h"
#include "ai.h"
#include "player.h"

#define PLAYER 1
#define AI 2

#define ROW_COUNT 6
#define COLUMN_COUNT 7

#define true 1
#define false 0

#define LOW_VALUE -1000000000
#define HIGH_VALUE 1000000000

int changeTurn(int turn) {
    if(turn == PLAYER) return AI;
    else return PLAYER;
}

int firstMove(int board[ROW_COUNT][COLUMN_COUNT]) {
    int nPieces = 0;
    for(int r=0; r<ROW_COUNT; r++) {
        for(int c=0; c<COLUMN_COUNT; c++) {
            if(board[r][c] != 0) nPieces++;
        }
    }

    if(nPieces == 0 || nPieces == 1) {
        return COLUMN_COUNT/2;
    }

    return false;
}

int main() {
    int board[ROW_COUNT][COLUMN_COUNT] = {0};
    int gameOver = false;
    int depth = 8;
    int turn = 1;

    printf("Enter the depth of minimax algorithm (the recommended value is 8): ");
    scanf("%d", &depth);

    printBoard(board);

    while(!gameOver) {
        if(turn == PLAYER) {
            int col = getPlayerColumn();
            dropPiece(board, col-1, PLAYER);
            printBoard(board);

            if(winningMove(board, PLAYER)) {
                printf("Player won!\n\n");
                gameOver = true;
            }

            turn = changeTurn(turn);
        }

        if(turn == AI && !gameOver) {
            int col;
            col = firstMove(board);
            if(col == 0) {col = (int)minimax(board, depth, (double)LOW_VALUE, (double)HIGH_VALUE, true)[0];}

            dropPiece(board, col, AI);
            printBoard(board);

            if(winningMove(board, AI)) {
                printf("AI won!\n\n");
                gameOver = true;
            }

            turn = changeTurn(turn);
        }

        if(isDraw(board) && !gameOver) {
            printf("Draw!\n\n");
            gameOver = true;
        }
    }
    
    return 0;
}
