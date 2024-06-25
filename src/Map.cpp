#include <iostream>

#include "Map.h"

using namespace std;

// 0 empty
// 1 wall
// 2 player
// 3 player's layzer
// 4 enemy
// 5 enemy's layzer

void Map::drawBuffer() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            switch (map_buffer[i][j]) {
                case 0: cout << " "; break;
                case 1: cout << "@"; break;
                case 2: cout << "+"; break;
                case 3: cout << "|"; break;
                case 4: cout << "+"; break;
                case 5: cout << "|"; break;
            }
        }
    }
}

void Map::initBuffer() {
    // copy map_data to map_buffer
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map_buffer[i][j] = map_data[i][j];
        }
    }    
}

char **Map::getBuffer() {
    return (char **) map_buffer;
}