#ifndef ENEMY
#define ENEMY

#include "Character.h"
#include "EnemyManager.h"

#define ENEMY_DIRECTION DOWN

class Enemy : public Character {
    private:
        EnemyManager *enemy_manager;
        int index;

    public:
        void getDamaged(int damage);

    public: Enemy(int x, int y, int hp, int layzer_speed, int power, EnemyManager *enemy_manager, int index) : Character(ENEMY_DIRECTION, x, y, hp, layzer_speed, power) {
        this->enemy_manager = enemy_manager;
        index = index;
    }

    public: ~Enemy() {
        enemy_manager->deleteEnemy(index);
    }
};

#endif