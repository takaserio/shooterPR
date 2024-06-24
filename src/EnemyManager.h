#ifndef ENEMYMANAGER
#define ENEMYMANAGER

#include "Enemy.h"
#include "body_data"
#include "NormalEnemy.h"

#define MAX_ENEMIES 100

class EnemyManager {
    private:
       Character *enemies[100];
       bool enemy_alive[100];

    public:
        void spawn(int x, int y, enum EnemyType enemy_type);
        void moveAllCharacters();
        void deleteEnemy(int index);
};

#endif