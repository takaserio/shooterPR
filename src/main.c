#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>

#define MAP_HEIGHT 30
#define MAP_WIDTH  30

#define MAX_LAYZERS 100 

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

struct Layzer {
    int x, y;
    enum Direction direction;
};

struct LayzerManager {
    struct Layzer *layzers[MAX_LAYZERS];
    int layzers_alive[MAX_LAYZERS];
};

struct Character {
    int x, y;
    int hp;
    int character_type;
    struct LayzerManager layzer_manager;
    enum Direction direction;
};

struct CharacterManager {
    struct Character *characters[100];
    int character_alive[100];
};

// 0 empty
// 1 wall
// 2 player
// 3 enemy
// 4 layzer
char map_data[MAP_HEIGHT][MAP_WIDTH];

void InitMapData(void) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if ((i == 0 || i == MAP_HEIGHT - 1) || (j == 0 || j == MAP_WIDTH - 1)) {
                map_data[i][j] = 1;
            } else {
                map_data[i][j] = 0;
            }
        }
    }
}

void InitCharacter(struct Character *character, int x, int y, enum Direction direction, int character_type) {
    character->x = x;
    character->y = y;
    character->direction = direction;
    character->character_type = character_type;
    character->hp = 100;

    for (int i = 0; i < MAX_LAYZERS; i++) {
        character->layzer_manager.layzers_alive[i] = 0;
    }
}

void InitCharacterManager(struct CharacterManager *character_manager) {
    for (int i = 0; i < 100; i++) {
        character_manager->character_alive[i] = 0;
    }
}

void MakeLayzer(struct Character *character) {
    for (int i = 0; i < MAX_LAYZERS; i++) {
        if (!character->layzer_manager.layzers_alive[i]) {
            character->layzer_manager.layzers[i] = malloc(sizeof(struct Layzer));
            character->layzer_manager.layzers_alive[i] = 1;

            // init layzer
            character->layzer_manager.layzers[i]->direction = character->direction;
            character->layzer_manager.layzers[i]->x = character->x;

            switch (character->direction) {
                case UP:
                    character->layzer_manager.layzers[i]->y = character->y - 2;
                    break;

                case DOWN:
                    character->layzer_manager.layzers[i]->y = character->y + 2;
            }

            break;
        }
    }
}

void moveAllEnemies(struct CharacterManager *character_manager) {
    for (int i = 0; i < 100; i++) {
        if ((character_manager->character_alive[i] == 1) && (character_manager->characters[i]->direction == DOWN)) {
            int random = rand() % 5;

            switch (random) {
                case 0 ... 2: // wait
                    break;

                case 3:
                    if (!NextIsWallCharacter(character_manager->characters[i], RIGHT)) {
                        character_manager->characters[i]->x += 1;
                    }
                    break;

                case 4:
                    if (!NextIsWallCharacter(character_manager->characters[i], LEFT)) {
                        character_manager->characters[i]->x -= 1;
                    }
                    break;
            }
        }   
    }
}

struct Character *WhoIsAtXY(struct CharacterManager *character_manager, int x, int y) {
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            if ((character_manager->characters[i]->x == x) && (character_manager->characters[i]->y == y)) {
                return character_manager->characters[i];
            }
        }
    }
}

void moveAllLayzer(struct CharacterManager *character_manager) {
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            for (int j = 0; j < MAX_LAYZERS; j++) {
                if (character_manager->characters[i]->layzer_manager.layzers_alive[j]) {
                    switch (character_manager->characters[i]->direction) {
                        case UP:
                            if (NextIsWallLayzer(character_manager->characters[i]->layzer_manager.layzers[j], character_manager->characters[i]->direction)) {
                                // delete layzer
                                DeleteLayzer(&(character_manager->characters[i]->layzer_manager), j);
                            } else {
                                character_manager->characters[i]->layzer_manager.layzers[j]->y -= 1;
                            }
                            break;
                        
                        case DOWN:
                            if (NextIsWallLayzer(character_manager->characters[i]->layzer_manager.layzers[j], character_manager->characters[i]->direction)) {
                                DeleteLayzer(&(character_manager->characters[i]->layzer_manager), j);
                            } else {
                                character_manager->characters[i]->layzer_manager.layzers[j]->y += 1;
                            }
                            break;
                    }
                }
            }
        }   
    }
}

#define LAYZER_POWER 50

void getDamaged(struct CharacterManager *character_manager, int index) {
    character_manager->characters[index]->hp -= LAYZER_POWER;

    if (character_manager->characters[index]->hp < 1) {
        DeleteCharacter(character_manager, index);
    }
}

void DeleteLayzer(struct LayzerManager *layzer_manager, int index) {
    layzer_manager->layzers_alive[index] = 0;
    free(layzer_manager->layzers[index]);
}

int NextIsWallLayzer(struct Layzer *layzer, enum Direction direction) {
    switch (direction) {
        case UP  : return (map_data[layzer->y - 1][5] == 1);
        case DOWN: return (map_data[layzer->y + 1][5] == 1);
    }
}

int NextIsWallCharacter(struct Character *character, enum Direction direction) {
    switch (direction) {
        case UP   : return (map_data[character->y - 1][character->x] == 1);
        case DOWN : return (map_data[character->y + 1][character->x] == 1);
        case RIGHT: return (map_data[character->y][character->x + 1] == 1);
        case LEFT : return (map_data[character->y][character->x - 1] == 1);
    }
}

int NextIsCharacterLayzer(struct Layzer *layzer, enum Direction direction, struct CharacterManager *character_manager) {
    int layzer_x = layzer->x;
    int layzer_y = layzer->y;
    
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            int character_x = character_manager->characters[i]->x;
            int character_y = character_manager->characters[i]->y;

            switch (direction) {
                case UP:
                    if ((layzer_y - 1 == character_y) && (layzer_x == character_x)) return 1;
                    break;

                case DOWN:
                    if ((layzer_y + 1 == character_y) && (layzer_x == character_x)) return 1;
            }
        }
    }

    return 0;
    
    /*
    switch (direction) {
        case UP:
            break;
        case DOWN:
            break;
    }*/
}

void MovePlayer(struct Character *player, int *key_input, int pre_key_input) {
    switch (*key_input) {
        case 'w':
            if (!NextIsWallCharacter(player, UP)) {
                player->y -= 1;
            }
            break;

        case 's':
            if (!NextIsWallCharacter(player, DOWN)) {
                player->y += 1;
            }
            break;

        case 'd':
            if (!NextIsWallCharacter(player, RIGHT)) {
                player->x += 1;
            }
            break;

        case 'a':
            if (!NextIsWallCharacter(player, LEFT)) {
                player->x -= 1;
            }
            break;

        case ' ':
            MakeLayzer(player);
            *key_input = pre_key_input;
            MovePlayer(player, key_input, pre_key_input);
            break;
    }
}

draw(struct CharacterManager *character_manager) {
    // make a buffer displayed.
    char buffer[MAP_HEIGHT][MAP_WIDTH];

    // copy map_data to buffer
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            buffer[i][j] = map_data[i][j];
        }
    }

    // insert player and layzer 's data into buffer
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            int character_x = character_manager->characters[i]->x;
            int character_y = character_manager->characters[i]->y;
            buffer[character_y][character_x] = character_manager->characters[i]->character_type;

            for (int j = 0; j < MAX_LAYZERS; j++) {
                if (character_manager->characters[i]->layzer_manager.layzers_alive[j]) {
                    int layzer_x = character_manager->characters[i]->layzer_manager.layzers[j]->x;
                    int layzer_y = character_manager->characters[i]->layzer_manager.layzers[j]->y;
                    buffer[layzer_y][layzer_x] = 4;
                }
            }
        }
    }

    printf("\033[2J");

    // converting 2 decimal array into 1 decimal might be good.
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            switch (buffer[i][j]) {
                case 0: printf(" "); break;
                case 1: printf("@"); break;
                case 2: printf("+"); break;
                case 3: printf("+"); break;
                case 4: printf("|"); break;
                default: printf("?");
            }
        }

        printf("\n");
    }
}

void SpawnCharacter(struct CharacterManager *character_manager, int x, int y, enum Direction direction, int character_type) {
    for (int i = 0; i < 100; i++) {
        if (!character_manager->character_alive[i]) {
            character_manager->characters[i] = (struct Character *) malloc(sizeof(struct Character));
            character_manager->character_alive[i] = 1;

            InitCharacter(character_manager->characters[i], x, y, direction, character_type);

            break;
        }
    }    
}

void DeleteCharacter(struct CharacterManager *character_manager, int index) {
    free(character_manager->characters[index]);
    character_manager->character_alive[index] = 0;
}

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void InputThread(int *key_input) {
    while (1) {
        *key_input = getch();
    }
}

// If any layzers and a particular character is at same cordinate, call DeleteCharacter()
void Collision(struct CharacterManager *character_manager) {
    // specify a character
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            int character_x = character_manager->characters[i]->x;
            int character_y = character_manager->characters[i]->y;

            for (int j = 0; j < 100; j++) {
                if (character_manager->character_alive[j]) {
                    for (int k = 0; k < MAX_LAYZERS; k++) {
                        if (character_manager->characters[j]->layzer_manager.layzers_alive[k]) {
                            int layzer_x = character_manager->characters[j]->layzer_manager.layzers[k]->x;
                            int layzer_y = character_manager->characters[j]->layzer_manager.layzers[k]->y;

                            if ((character_x == layzer_x) && (character_y == layzer_y)) {
                                getDamaged(character_manager, i);
                            }
                        }
                    }
                }
            }
            
        }
    }
}

int main() {
    struct CharacterManager character_manager;
    int key_input = 0;
    int pre_key_input = 0;
    int win = 0;
    pthread_t thread_id;
    unsigned int msecounds = 100000;

    pthread_create(&thread_id, NULL, InputThread, &key_input);

    InitCharacterManager(&character_manager);
    InitMapData();

    SpawnCharacter(&character_manager, 15, 25, UP, 2); // index 0 is the player
    SpawnCharacter(&character_manager, 4, 4, DOWN, 3);
    SpawnCharacter(&character_manager, 11, 5, DOWN, 3);

    srand(time(NULL));

    while (!win) {
        moveAllLayzer(&character_manager);
        moveAllEnemies(&character_manager);
        MovePlayer(character_manager.characters[0], &key_input, pre_key_input);

        Collision(&character_manager);

        draw(&character_manager);

        pre_key_input = key_input;
        usleep(msecounds);
    }
}