#ifndef MAP
#define MAP

#define MAP_WIDTH  30
#define MAP_HEIGHT 30

class Map {
    private:
        // 0 empty
        // 1 wall
        // 2 player
        // 3 player's layzer
        // 4 enemy
        // 5 enemy's layzer
        char map_data[MAP_HEIGHT][MAP_WIDTH];
        char map_buffer[MAP_HEIGHT][MAP_WIDTH];
        int map_number;

    public:
        void drawBuffer();
        void initBuffer();
        char **getBuffer();
        Map(int map_number);
};

#endif