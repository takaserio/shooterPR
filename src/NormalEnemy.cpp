#include <cstdlib>

#include "NormalEnemy.h"

void NormalEnemy::move() {
    int rand = std::rand() % 30;
    
    // 4/30  UP
    // 4/30  DOWN
    // 11/30 RIGHT
    // 11/30 LEFT
    switch (rand) {
        case 0 ... 3: // UP
            break;

        case 4 ... 7: // DOWN
            break;

        case 8 ... 18: // RIGHT
            break;

        case 19 ... 29: // LEFT
    }
}