#ifndef NORMALENEMY
#define NORMALENEMY

#include "Enemy.h"

#define NORMAL_HP 100
#define NORMAL_LAYZER_SPEED 1
#define NORMAL_POWER 50

class NormalEnemy : public Enemy {
    private:

    public:
        void move(char **map_data);
        NormalEnemy(int x, int y) : Enemy(x, y, NORMAL_HP, NORMAL_LAYZER_SPEED, NORMAL_POWER) {
            body = (char **) normal_enemy;
        };
};

#endif