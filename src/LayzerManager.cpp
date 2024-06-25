#include "LayzerManager.h"
#include "Layzer.h"

LayzerManager::LayzerManager() {};

LayzerManager::~LayzerManager() {
    for (int i = 0; i < MAX_LAYZER; i++) {
        if (layzerAlive[i]) {
            delete layzers[i];
        }
    }
}

void LayzerManager::spawn(int x, int y, enum Direction direction, int speed, int damage) {
    for (int i = 0; i < MAX_LAYZER; i++) {
        if (!layzerAlive[i]) {
            layzers[i] = new Layzer(x, y, direction, speed, damage);

            break;
        }
    }

    // ERROR
}

void LayzerManager::moveAllLayzer() {
    for (int i = 0; i < MAX_LAYZER; i++) {
        if (layzerAlive[i]) {
            layzers[i]->move();
        }
    }
}