#ifndef BODY_DATA
#define BODY_DATA

// 0 empty
// 1 wall
// 2 player
// 3 player's layzer
// 4 enemy
// 5 enemy's layzer

// these value only can be odds
#define CHARACTER_BODY_WIDTH  5
#define CHARACTER_BODY_HEIGHT 5

#define LAYZER_BODY_WIDHT  3
#define LAYZER_BODY_HEIGHT 3

enum LayzerType {
    BASIC,
};

enum EnemyType {
    NORMAL,
};

class BodyDataManager {
    public:
        char **getBody(int);
};

#endif