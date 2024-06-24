#ifndef PLAYER
#define PLAYER

#include "Character.h"
#include "LayzerManager.h"

#define PLAYER_DIRECTION UP
#define PLAYER_INIT_X 5
#define PLAYER_INIT_Y 5
#define PLAYER_HP 100
#define PLAYER_LAYZER_SPEED 1
#define PLAYER_POWER 50

class Player : public Character {
    private:

    public:
        void move(int key_input);

    public: Player() : Character(PLAYER_DIRECTION, PLAYER_INIT_X, PLAYER_INIT_Y, PLAYER_HP, PLAYER_LAYZER_SPEED, PLAYER_POWER) {
            body = (char **) player;
        }
};

#endif