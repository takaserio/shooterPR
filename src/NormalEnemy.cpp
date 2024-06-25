#include <cstdlib>

#include "NormalEnemy.h"

/*
NormalEnemy::NormalEnemy(int x, int y) : Enemy(x, y, NORMAL_HP, NORMAL_LAYZER_SPEED, NORMAL_POWER) {
    body = (char **) normal_enemy;
}*/

void NormalEnemy::move(char **map_data) {
    int rand = std::rand() % 60;
    
    // 4/60  UP
    // 4/60  DOWN
    // 11/60 RIGHT
    // 11/60 LEFT
    //
    switch (rand) {
        case 0 ... 3: // UP
            if (!nextIsWall(UP, map_data)) {
                y -= 1;
            }
            break;

        case 4 ... 7: // DOWN
            if (!nextIsWall(DOWN, map_data)) {
                y += 1;
            }
            break;

        case 8 ... 18: // RIGHT
            if (!nextIsWall(RIGHT, map_data)) {
                x += 1;
            }
            break;

        case 19 ... 29: // LEFT
            if (!nextIsWall(LEFT, map_data)) {
                x -= 1;
            }
            break;

        case 30 ... 59:
            break;
    }
}