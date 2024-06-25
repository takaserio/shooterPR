#ifndef CHARACTER
#define CHARACTER

#include "LayzerManager.h"
#include "DirectionEnum"
#include "body_data"


class LayzerManager {
    
};

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
        virtual void move();
        virtual void getDamaged(int damage);  // invoked when a collision of a layzer hit this character
        int getLayzerSpeed();
        bool nextIsWall(enum Direction direction, char **map_data);
        int getPower();
        void updateBuffer(char **map);
        int getX();
        int getY();

    public: Character(enum Direction direction, int x, int y, int hp, int layzer_speed, int power) {
        direction = direction;
        this->layzer_manager = new LayzerManager(this);
        this->x = x;
        this->y = y;
        this->hp = hp;
        this->layzer_speed = layzer_speed;
        this->power = power;

        dead = false;
    }

    public: ~Character() {
        delete layzer_manager;
    }
};  

#endif