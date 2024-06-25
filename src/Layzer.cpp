#include "Layzer.h"

Layzer::Layzer(int x, int y, enum Direction direction, int speed, int damage) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    this->speed = speed;
    this->damage = damage;
}

void Layzer::move() {
    switch (direction) {
        case UP:   y -= 1; break;
        case DOWN: y += 1; break;
    }
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