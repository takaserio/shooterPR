#include "LayzerManager.h"
#include "Layzer.h"

void LayzerManager::spawn(int x, int y) {
    for (int i = 0; i < MAX_LAYZER; i++) {
        if (!layzerAlive[i]) {
            layzers[i] = new Layzer(x, y, owner);

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