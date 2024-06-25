#include "Layzer.h"

Layzer::Layzer(int x, int y, Character *owner) {
    this->x = x;
    this->y = y;
    this->owner = owner;
    this->direction = owner->direction;
    speed = owner->getLayzerSpeed();
    damage = owner->getPower();
}

void Layzer::updateBuffer(char **map_buffer) {
    int offset_y = LAYZER_BODY_HEIGHT / 2 - LAYZER_BODY_HEIGHT - 1;
    int offset_x = LAYZER_BODY_WIDHT  / 2 - LAYZER_BODY_WIDHT - 1;

    for (int i = 0; i < LAYZER_BODY_HEIGHT; i++) {
        for (int j = 0; j < LAYZER_BODY_WIDHT; j++) {
            int y = (y + offset_y) - 1;
            int x = (x + offset_x) - 1;

            map_buffer[y][x] = body[i][j];
        }
    }
}