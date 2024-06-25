#include "EnemyManager.h"

void EnemyManager::spawn(int x, int y, enum EnemyType enemy_type) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemy_alive[i]) {
            switch (enemy_type) {
                case NORMAL:
                    enemy_alive[i] = new NormalEnemy(x, y, this, i);
            }
        }
    }
}

void EnemyManager::deleteEnemy(int index) {
    delete enemies[index];
    enemy_alive[index] = false;
}

void EnemyManager::moveAllEnemies() {
    
}

void EnemyManager::updateBuffer(char **map_buffer) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemy_alive[i]) {
        }
    }
}