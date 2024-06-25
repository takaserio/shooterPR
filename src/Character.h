#ifndef CHARACTER
#define CHARACTER

#include "LayzerManager.h"
#include "DirectionEnum"
#include "BodyDataManager.h"

class Character {
    protected:
        // the x and y represent a core of a character
        int x, y;
        int hp;
        int layzer_speed;
        int power;
        bool dead;
        LayzerManager *layzer_manager;
        char **body;

    public:
        enum Direction direction;
        enum LayzerType layzer_type;

    public:
        bool isDead();
        void layzerAttack(); // make a layzer
        void move();
        void getDamaged(int damage);  // invoked when a collision of a layzer hit this character
        int getLayzerSpeed();
        bool nextIsWall(enum Direction direction, char **map_data);
        int getPower();
        void updateBuffer(char **map);
        int getX();
        int getY();

        Character(enum Direction direction, int x, int y, int hp, int layzer_speed, int power);
        ~Character();
};  

#endif