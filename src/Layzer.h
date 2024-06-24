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

        // attack
        void collision();
        void die();

    public: Layzer(int x, int y, Character *owner) {
        x = x;
        y = y;
        owner = owner;
        direction = owner->direction;
        speed = owner->getLayzerSpeed();
        damage = owner->getPower();
    }
};

#endif