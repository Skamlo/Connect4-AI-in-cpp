#include <stdio.h>

#define true 1

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
