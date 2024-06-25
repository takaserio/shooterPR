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
        void moveAllEnemies();
        void deleteEnemy(int index);
        void updateBuffer(char **map_buffer);

        EnemyManager() {
            for (int i = 0; i < MAX_ENEMIES; i++) {
                enemy_alive[i] = false;
            }
        }
};

#endif