#ifndef NORMALENEMY
#define NORMALENEMY

#include "Enemy.h"
#include "EnemyManager.h"

#define NORMAL_HP 100
#define NORMAL_LAYZER_SPEED 1
#define NORMAL_POWER 50

class NormalEnemy : public Enemy {
    private:

    public:
        void move(char **map_data);

    public: NormalEnemy(int x, int y, EnemyManager *enemy_manager, int index) : Enemy(x, y, NORMAL_HP, NORMAL_LAYZER_SPEED, NORMAL_POWER, enemy_manager, index) {
            body = (char **) normal_enemy;
        }
};

#endif