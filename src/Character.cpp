#include "Character.h"

Character::Character(enum Direction direction, int x, int y, int hp, int layzer_speed, int power) {
    this->direction = direction;
    this->layzer_manager = new LayzerManager;
    this->x = x;
    this->y = y;
    this->hp = hp;
    this->layzer_speed = layzer_speed;
    this->power = power;

    dead = false;
}

Character::~Character() {
    delete layzer_manager;
    delete this;
}

int Character::getX() {
    return x;
}

int Character::getY() {
    return y;
}

void Character::layzerAttack() {
    layzer_manager->spawn(x, y+2, direction, layzer_speed, power);
}

bool Character::nextIsWall(enum Direction direction, char **map_data) {
    switch (direction) {
        case UP:
            for (int i = 0; i < CHARACTER_BODY_HEIGHT; i++) {
                for (int j = 0; j < CHARACTER_BODY_WIDTH; j++) {
                    if (body[i][j] != ' ') {
                        int x_offset = CHARACTER_BODY_WIDTH  / 2 - CHARACTER_BODY_WIDTH  - 1;
                        int y_offset = CHARACTER_BODY_HEIGHT / 2 - CHARACTER_BODY_HEIGHT - 1;

                        int x = (this->x + x_offset) - 1;
                        int y = (this->y + y_offset) - 1;

                        if (map_data[y + 1][x] == 1) {
                            return true;
                        }

                        return false;
                    }
                }
            }
            break;

        case DOWN:
            for (int i = CHARACTER_BODY_HEIGHT - 1; i >= 0; i--) {
                for (int j = CHARACTER_BODY_WIDTH; j >= 0; j--) {
                    if (body[i][j] != ' ') {
                        int x_offset = CHARACTER_BODY_WIDTH  / 2 - CHARACTER_BODY_WIDTH  - 1;
                        int y_offset = CHARACTER_BODY_HEIGHT / 2 - CHARACTER_BODY_HEIGHT - 1;

                        int x = (this->x + x_offset) - 1;
                        int y = (this->y + y_offset) - 1;

                        if (map_data[y - 1][x] == 1) {
                            return true;
                        }

                        return false;
                    }
                }
            }
            break;

        case RIGHT:
            for (int i = 0; i < CHARACTER_BODY_WIDTH; i++) {
                for (int j = 0; j < CHARACTER_BODY_HEIGHT; j++) {
                    if (body[i][j] != ' ') {
                        int x_offset = CHARACTER_BODY_WIDTH  / 2 - CHARACTER_BODY_WIDTH  - 1;
                        int y_offset = CHARACTER_BODY_HEIGHT / 2 - CHARACTER_BODY_HEIGHT - 1;

                        int x = (this->x + x_offset) - 1;
                        int y = (this->y + y_offset) - 1;

                        if (map_data[y][x + 1] == 1) {
                            return true;
                        }

                        return false;
                    }
                }
            }
            break;

        case LEFT:
            for (int i = CHARACTER_BODY_WIDTH - 1; i >= 0; i--) {
                for (int j = CHARACTER_BODY_HEIGHT - 1; j >= 0; j--) {
                    if (body[i][j] != ' ') {
                        int x_offset = CHARACTER_BODY_WIDTH  / 2 - CHARACTER_BODY_WIDTH  - 1;
                        int y_offset = CHARACTER_BODY_HEIGHT / 2 - CHARACTER_BODY_HEIGHT - 1;

                        int x = (this->x + x_offset) - 1;
                        int y = (this->y + y_offset) - 1;

                        if (map_data[y][x - 1] == 1) {
                            return true;
                        }

                        return false;
                    }
                }
            }
            break;
    }
}

void Character::updateBuffer(char **map_buffer) {
    int offset_y = CHARACTER_BODY_HEIGHT / 2 - CHARACTER_BODY_HEIGHT - 1;
    int offset_x  = CHARACTER_BODY_WIDTH  / 2 - CHARACTER_BODY_WIDTH  - 1;

    for (int i = 0; i < CHARACTER_BODY_HEIGHT; i++) {
        for (int j = 0; j < CHARACTER_BODY_WIDTH; j++) {
            int y = (y + offset_y) - 1;
            int x = (x + offset_x)  - 1;

            map_buffer[y][x] = body[i][j];
        }
    }
}