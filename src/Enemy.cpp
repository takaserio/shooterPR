#include "Enemy.h"

void Enemy::getDamaged(int damage) {
    hp -= damage;

    if (hp < 1) {
        // DeLEt mYSelF
        this->~Enemy();
    }
}