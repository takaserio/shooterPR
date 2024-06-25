#ifndef LAYZERMANAGER
#define LAYZERMANAGER

#include "Layzer.h"

#define MAX_LAYZER 100

class LayzerManager {
    private:
        Layzer *layzers[MAX_LAYZER];
        bool layzerAlive[MAX_LAYZER];

    public:
        void spawn(int x, int y, enum Direction direction, int speed, int damage);
        void moveAllLayzer();
        //void killLayzer(int index);

    public: LayzerManager();
    public: ~LayzerManager();
};

#endif