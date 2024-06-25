#ifndef LAYZER
#define LAYZER

//#include "Character.h"
#include "DirectionEnum"

class Layzer {
    private:
        int x, y;
        char **body;
        int speed;
        int direction;
        int damage;

    public:
        void move();

        // attackd
        void collision();
        void die();
        void updateBuffer(char **);

    public: Layzer(int x, int y, enum Direction direction, int speed, int damage);
};

#endif