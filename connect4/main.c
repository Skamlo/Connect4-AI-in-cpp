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

int changeTurn(int turn) {
    if(turn == PLAYER) {
        return AI;
    }
    else {
        return PLAYER;
    }
}

int main() {
    int board[ROW_COUNT][COLUMN_COUNT] = {0};
    printBoard(board);

    int turn = 1;

    while(true) {
        if(turn == PLAYER) {
            int col = getPlayerColumn();
            dropPiece(board, col, PLAYER);
            printBoard(board);

            if(winningMove(board, PLAYER)) {
                printf("Player won!\n\n");
                break;
            }

            turn = changeTurn(turn);
        }
        else if(turn == AI) {
            int col = randInt(1, 7);
            dropPiece(board, col, AI);
            printBoard(board);

            if(winningMove(board, AI)) {
                printf("AI won!\n\n");
                break;
            }

            turn = changeTurn(turn);
        }
    }
    
}
