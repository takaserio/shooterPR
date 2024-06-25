#include "Player.h"

/*
Player::Player() : Character(PLAYER_DIRECTION, PLAYER_INIT_X, PLAYER_INIT_Y, PLAYER_HP, PLAYER_LAYZER_SPEED, PLAYER_POWER) {
    body = (char **) player;
}*/

void Player::move(int key_input, char **map_data) {
    switch (key_input) {
        case 'w':
            if (!nextIsWall(UP, map_data)) {
                y -= 1;
            }
            break;

        case 'd':
            if (!nextIsWall(RIGHT, map_data)) {
                x += 1;
            }
            break;

        case 's':
            if (!nextIsWall(DOWN, map_data)) {
                y += 1;
            }
            break;

        case 'a':
            if (!nextIsWall(LEFT, map_data)) {
                x -= 1;
            }
            break;
        
        case ' ':
            layzerAttack();
    }
}

void Player::getDamaged(int damage) {
    
}