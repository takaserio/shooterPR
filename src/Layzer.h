#ifndef LAYZER
#define LAYZER

#include "Character.h"
#include "body_data"

class Layzer {
    private:
        int x, y;
        Character *owner;
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

    public: Layzer(int x, int y, Character *owner);
};

#endif