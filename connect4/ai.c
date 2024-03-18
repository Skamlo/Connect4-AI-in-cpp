#include <stdlib.h>
#include <time.h>

int randInt(int low, int high) {
    srand(time(NULL));
    int random_number = low + rand() % (high - low + 1);
    return random_number;
}
