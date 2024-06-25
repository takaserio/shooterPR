#ifndef ENEMY
#define ENEMY

#include "Character.h"

#define ENEMY_DIRECTION DOWN

class Enemy : public Character {
    public:
        void getDamaged(int damage);
        Enemy(int x, int y, int hp, int layzer_speed, int power) : Character(ENEMY_DIRECTION, x, y, hp, layzer_speed, power) {};

};

#endif