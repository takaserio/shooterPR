#ifndef LAYZERMANAGER
#define LAYZERMANAGER

#include "Layzer.h"

#define MAX_LAYZER 100

class LayzerManager {
    private:
        Layzer *layzers[MAX_LAYZER];
        bool layzerAlive[MAX_LAYZER];

    public:
        Character *owner;

    public:
        void spawn(int x, int y);
        void moveAllLayzer();
        //void killLayzer(int index);

    public: LayzerManager(Character *owner) {
        owner = owner;
    }

    public: ~LayzerManager() {
        for (int i = 0; i < MAX_LAYZER; i++) {
            if (layzerAlive[i]) {
                delete layzers[i];
            }
        }
    }
};

#endif