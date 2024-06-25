#include "BodyDataManager.h"

char basic_layzer[LAYZER_BODY_HEIGHT][LAYZER_BODY_WIDHT] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '},
};

char normal_enemy[CHARACTER_BODY_HEIGHT][CHARACTER_BODY_WIDTH] = {
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
};

char player[CHARACTER_BODY_HEIGHT][CHARACTER_BODY_WIDTH] = {
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
    {' ', ' ', ' ', ' ', ' ', },
};


char **BodyDataManager::getBody(int body_number) {
    switch (body_number) {
        case 0: return (char **) player; break;
        case 1: return (char **) normal_enemy; break;
        case 2: return (char **) basic_layzer; break;
    }
}