#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "data"

#define MAP_HEIGHT 30
#define MAP_WIDTH  30

#define MAX_LAYZERS 100 

// CharacterManager can hold reference to character equal to MAX_CHARACTERS
#define MAX_CHARACTERS 100

#define ITEM_NUM 3
struct InfoDisplay {
    int itemNum;
    int score;
    int playerHp;
    char *msg;
};

struct Layzer {
    int x, y;
    int leftX, topY;
    char (*body)[LAYZER_WIDTH][DATA_DISPLAYED];
    enum Direction direction;
};

struct LayzerManager {
    struct Layzer *layzers[MAX_LAYZERS];
    int layzers_alive[MAX_LAYZERS];
};

struct Character {
    int x, y; // core
    int leftX, topY;
    char (*body)[CHARACTER_WIDTH][DATA_DISPLAYED];
    int hp;
    struct LayzerManager layzer_manager;
    enum Direction direction;
    enum LayzerType layzerType;
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
char map_data[MAP_HEIGHT][MAP_WIDTH][DATA_DISPLAYED];

void initInfoDisplay(struct InfoDisplay *infoDisplay) {
    infoDisplay->itemNum = ITEM_NUM;
    infoDisplay->score = 0;
    infoDisplay->msg = 0;
    infoDisplay->playerHp = 0;
}

void InitMapData(void) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if ((i == 0 || i == MAP_HEIGHT - 1) || (j == 0 || j == MAP_WIDTH - 1)) {
                strcpy(map_data[i][j], "@");
            } else {
                strcpy(map_data[i][j], " ");
            }
        }
    }
}

void InitCharacter(struct Character *character, int x, int y, enum Direction direction, enum CharacterType characterType, int hp, enum LayzerType layzerType) {
    character->x = x;
    character->y = y;
    character->direction = direction;
    character->hp = hp;
    character->leftX = x - 2;
    character->topY  = y - 2;
    character->layzerType = layzerType;

    switch (characterType) {
        case PLAYER: // Player
            character->body = player;
            break;

        case KAI:
            character->body = Kai;
            break;
    }

    for (int i = 0; i < MAX_LAYZERS; i++) {
        character->layzer_manager.layzers_alive[i] = 0;
    }
}

void InitCharacterManager(struct CharacterManager *character_manager) {
    for (int i = 0; i < 100; i++) {
        character_manager->character_alive[i] = 0;
    }
}

void DeleteLayzer(struct LayzerManager *layzer_manager, int index) {
    layzer_manager->layzers_alive[index] = 0;
    free(layzer_manager->layzers[index]);
}

void DeleteCharacter(struct CharacterManager *character_manager, int index) {
    free(character_manager->characters[index]);
    character_manager->character_alive[index] = 0;
}

int NextIsWall(int x, int y, enum Direction direction) {
    switch (direction) {
        case UP   : return (!strcmp(map_data[y - 1][x], "@"));
        case DOWN : return (!strcmp(map_data[y + 1][x], "@"));
        case RIGHT: return (!strcmp(map_data[y][x + 1], "@"));
        case LEFT : return (!strcmp(map_data[y][x - 1], "@"));
    }
}

#define LAYZER_POWER 50

void getDamaged(struct CharacterManager *character_manager, int character_index) {
    character_manager->characters[character_index]->hp -= LAYZER_POWER;

    if (character_manager->characters[character_index]->hp < 1) {
        DeleteCharacter(character_manager, character_index);
    }
}

// Layzer use this
int NextIsCharacter(struct Layzer *layzer, enum Direction direction, struct CharacterManager *character_manager) {
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

void MakeLayzer(struct Character *character) {
    for (int i = 0; i < MAX_LAYZERS; i++) {
        if (!character->layzer_manager.layzers_alive[i]) {
            character->layzer_manager.layzers[i] = malloc(sizeof(struct Layzer));
            character->layzer_manager.layzers_alive[i] = 1;

            // shortcut refe
            struct Layzer *layzerPtr = character->layzer_manager.layzers[i];

            // init layzer
            layzerPtr->direction = character->direction;
            layzerPtr->x = character->x;

            switch (character->direction) {
                case UP:
                    layzerPtr->y = character->y - 2;
                    break;

                case DOWN:
                    layzerPtr->y = character->y + 2;
            }

            layzerPtr->leftX = layzerPtr->x - 1;
            layzerPtr->topY = layzerPtr->y - 1;

            switch (character->layzerType) {
            case GREENBEAM:
                layzerPtr->body = GreenBeam;
                break;

            case REDBEAM:
                layzerPtr->body = RedBeam;
                break;
            
            default:
                printf("ERROR: layzer body not fould\n");
                sleep(3);
            }

            break;
        }
    }
}

void MovePlayer(struct Character *player, int *key_input, int pre_key_input) {
    switch (*key_input) {
        case 'w':
            if (!NextIsWall(player->x, player->y, UP)) {
                player->y -= 1;
                player->topY -= 1;
            }
            break;

        case 's':
            if (!NextIsWall(player->x, player->y, DOWN)) {
                player->y += 1;
                player->topY += 1;
            }
            break;

        case 'd':
            if (!NextIsWall(player->x, player->y, RIGHT)) {
                player->x += 1;
                player->leftX += 1;
            }
            break;

        case 'a':
            if (!NextIsWall(player->x, player->y, LEFT)) {
                player->x -= 1;
                player->leftX -= 1;
            }
            break;

        case ' ':
            MakeLayzer(player);
            *key_input = pre_key_input;
            MovePlayer(player, key_input, pre_key_input);
            break;
    }
}

// In the future, I think all enemies have their own move patterns.
void moveAllCharacters(struct CharacterManager *character_manager, int *key_input, int pre_key_input) {
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        // only the direction of Player is UP so the loop hasn't impact on Player
        if ((character_manager->character_alive[i] == 1) && (character_manager->characters[i]->direction == DOWN)) {
            // shortcut reference
            struct Character *characterPtr = character_manager->characters[i];

            switch (rand() % 6) {
                case 0 ... 2: // wait
                    break;

                case 3: // RIGHT
                    if (!NextIsWall(characterPtr->x, characterPtr->y, RIGHT)) {
                        character_manager->characters[i]->x += 1;
                        character_manager->characters[i]->leftX += 1;
                    }
                    break;

                case 4: // LEFT
                    if (!NextIsWall(characterPtr->x, characterPtr->y, LEFT)) {
                        character_manager->characters[i]->x -= 1;
                        character_manager->characters[i]->leftX -= 1;
                    }
                    break;

                case 5: // Layzer
                    MakeLayzer(characterPtr);
                    break;
            }
        } else if ((character_manager->character_alive[i] == 1) && (character_manager->characters[i]->direction == UP)) {
            MovePlayer(character_manager->characters[0], key_input, pre_key_input);
        }
    }
}

// This name reminds me ARP... I miss you.
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
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_manager->character_alive[i]) {
            // shortcut reference
            struct Character *characterPtr = character_manager->characters[i];

            for (int j = 0; j < MAX_LAYZERS; j++) {
                if (characterPtr->layzer_manager.layzers_alive[j]) {
                    // shortcut reference
                    struct Layzer *layzerPtr = characterPtr->layzer_manager.layzers[j];

                    switch (layzerPtr->direction) {
                        case UP:
                            if (NextIsWall(layzerPtr->x, layzerPtr->y, layzerPtr->direction)) {
                                // delete layzer
                                DeleteLayzer(&(characterPtr->layzer_manager), j);
                            } else {
                                // move normaly
                                layzerPtr->y -= 1;
                                layzerPtr->topY -= 1;
                            }
                            break;
                        
                        case DOWN:
                            if (NextIsWall(layzerPtr->x, layzerPtr->y, layzerPtr->direction)) {
                                DeleteLayzer(&(characterPtr->layzer_manager), j);
                            } else {
                                layzerPtr->y += 1;
                                layzerPtr->topY += 1;
                            }
                            break;
                    }
                }
            }
        }   
    }
}

void drawInfoDisplay(struct InfoDisplay *infoDisplay) {
    printf("SCORE: %d\n", infoDisplay->score);
    printf("HP   : %d\n", infoDisplay->playerHp);
    printf("MSG: %s\n", infoDisplay->msg);
    printf("\n");
}

void updateBuffer(struct InfoDisplay *infoDisplay, int score, int playerHp, char *msg) {
    infoDisplay->score = score;
    infoDisplay->playerHp = playerHp;
    infoDisplay->msg = msg;
}

void drawMapBuffer(struct CharacterManager *character_manager) {
    // make a buffer displayed.
    char buffer[MAP_HEIGHT][MAP_WIDTH][100];

    // copy map_data to buffer
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            strcpy(buffer[i][j], map_data[i][j]);
        }
    }

    // insert player and layzer 's data into buffer
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            // shortcut refe
            struct Character *characterPtr = character_manager->characters[i];
            int character_x = character_manager->characters[i]->x;
            int character_y = character_manager->characters[i]->y;

            // draw character
            for (int offsetY = 0; offsetY < CHARACTER_HEIGHT; offsetY++) {
                for (int offsetX = 0; offsetX < CHARACTER_WIDTH; offsetX++) {
                    if (characterPtr->body[offsetY][offsetX][0] != ' ') {
                        strcpy(buffer[characterPtr->topY + offsetY][characterPtr->leftX + offsetX],
                               characterPtr->body[offsetY][offsetX]);
                    }
                }
            }

            // draw layzers
            for (int j = 0; j < MAX_LAYZERS; j++) {
                if (characterPtr->layzer_manager.layzers_alive[j]) {
                    // shortcut refe
                    struct Layzer *layzerPtr = characterPtr->layzer_manager.layzers[j];

                    for (int offsetY = 0; offsetY < LAYZER_HEIGHT; offsetY++) {
                        for (int offsetX = 0; offsetX < LAYZER_WIDTH; offsetX++) {
                            if (layzerPtr->body[offsetY][offsetX][0] != ' ') {
                                strcpy(buffer[layzerPtr->topY + offsetY][layzerPtr->leftX + offsetX],
                                       layzerPtr->body[offsetY][offsetX]);
                            }
                        }
                    }
                }
            }
        }
    }

    // converting 2 decimal array into 1 decimal might be good.
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            printf("%s", buffer[i][j]);
        }

        printf("\n");
    }
}

void draw(struct InfoDisplay *infoDisplay, struct CharacterManager *characterManager) {
    // clear screen
    printf("\033[2J");

    drawInfoDisplay(infoDisplay);
    drawMapBuffer(characterManager);
}

void SpawnCharacter(struct CharacterManager *character_manager, int x, int y, enum Direction direction, enum CharacterType character_type, int hp, enum LayzerType layzerType) {
    for (int i = 0; i < 100; i++) {
        if (!character_manager->character_alive[i]) {
            character_manager->characters[i] = (struct Character *) malloc(sizeof(struct Character));
            character_manager->character_alive[i] = 1;

            InitCharacter(character_manager->characters[i], x, y, direction, character_type, hp, layzerType);

            break;
        }
    }    
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

void InputThread (int *key_input) {
    while (1) {
        *key_input = getch();
    }
}

// If any layzers and a particular character is at same cordinate, call DeleteCharacter()
void Collision(struct CharacterManager *character_manager) {
    // specify a character
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_manager->character_alive[i]) {
            // shortcut reference
            struct Character *characterPtr1 = character_manager->characters[i];

            int character_x = characterPtr1->x;
            int character_y = characterPtr1->y;

            for (int j = 0; j < MAX_CHARACTERS; j++) {
                if (character_manager->character_alive[j]) {
                    // shortcut reference
                    struct Character *characterPtr2 = character_manager->characters[j];

                    for (int k = 0; k < MAX_LAYZERS; k++) {
                        if (characterPtr2->layzer_manager.layzers_alive[k]) {
                            // shortcut reference
                            struct Layzer *layzerPtr = characterPtr2->layzer_manager.layzers[k];

                            int layzer_x = layzerPtr->x;
                            int layzer_y = layzerPtr->y;

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

void UpdateInfoDisplay(struct InfoDisplay *infoDisplay, int score, int hp, char *msg) {
    infoDisplay->score = score;
    infoDisplay->msg = msg;
    infoDisplay->playerHp = hp;
}

int main() {
    struct CharacterManager character_manager;
    struct InfoDisplay infoDisplay;
    int key_input = 0;
    int pre_key_input = 0;
    int win = 0;
    int score = 0;
    pthread_t thread_id;
    unsigned int msecounds = 100000;

    srand(time(NULL));
    pthread_create(&thread_id, NULL, InputThread, &key_input);

    initInfoDisplay(&infoDisplay);
    InitCharacterManager(&character_manager);
    InitMapData();

    SpawnCharacter(&character_manager, 15, 25, UP, PLAYER, 100, GREENBEAM); // index 0 is the player
    SpawnCharacter(&character_manager, 4, 4, DOWN, KAI, 100, REDBEAM);
    SpawnCharacter(&character_manager, 11, 5, DOWN, KAI, 100, REDBEAM);

    struct Character *player = character_manager.characters[0];

    while (!win) {
        moveAllLayzer(&character_manager);
        moveAllCharacters(&character_manager, &key_input, pre_key_input);

        Collision(&character_manager);
        UpdateInfoDisplay(&infoDisplay, score, player->hp, NULL);

        draw(&infoDisplay, &character_manager);

        pre_key_input = key_input;
        usleep(msecounds);

        if (!character_manager.character_alive[0]) {
            printf("----- GAME OVER -----\n");
            win = 1;
            return 0;
        }

        for (int i = 1; i < MAX_CHARACTERS; i++) {
            if (character_manager.character_alive[i]) {
                break;
            } else if (i == MAX_CHARACTERS - 1) {
                printf("----- VICTORY -----\n");
                win = 1;
                return 0;
            }
        }
    }

    return 0;
}