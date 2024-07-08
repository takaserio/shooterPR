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

// LayzerManager can hold references to layzers equal number to MAX_CHARACTERS
#define MAX_LAYZERS 100

// CharacterManager can hold references to characters equal number to MAX_CHARACTERS
#define MAX_CHARACTERS 100

#define ITEM_NUM 3
struct InfoDisplay {
    int itemNum;
    int score;
    int playerHp;
    char *msg;
};

// Abstract structure to handle
// characters, layzers by collision()
struct Object {
	int x, y; // core coodinates of a body
	int left_x, top_y;
    char (*body)[OBJECT_WIDTH][CHARS];
    //int body_width; all objects have OBJECT_HEIGHT and OBJECT_WDITH in common, meaning these variables is not used.
    //int body_height;
};

struct Layzer {
    struct Object object;
    //char (*body)[LAYZER_WIDTH][DATA_DISPLAYED];
    enum Direction direction;
};

struct LayzerManager {
    struct Layzer *layzers[MAX_LAYZERS];
    int layzers_alive[MAX_LAYZERS];
};

// LeftX and topY might confusing.
// To understand what are they, reading drawMapBuffer() is good.
struct Character {
    struct Object object;
    //char (*body)[CHARACTER_WIDTH][DATA_DISPLAYED];
    int hp;
    int layzer_power;
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
char map_data[MAP_HEIGHT][MAP_WIDTH][CHARS];

void InitInfoDisplay(struct InfoDisplay *infoDisplay) {
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

void InitObject(struct Object *object, int x, int y , char (*body)[OBJECT_WIDTH][CHARS]) {
	object->x = x;
	object->y = y;
	object->left_x = x - OBJECT_WIDTH / 2;
	object->top_y = y - OBJECT_HEIGHT / 2;
    object->body = body;
}

void InitCharacter(struct Character *character, int x, int y, enum Direction direction, enum CharacterType characterType, int hp, enum LayzerType layzerType) {
    character->direction = direction;
    character->hp = hp;
    character->layzerType = layzerType;

    switch (characterType) {
        case PLAYER:
            InitObject(&character->object, x, y, player);
            //character->body = player;
            break;

        case KAI:
            //character->body = Kai;
            InitObject(&character->object, x, y, Kai);
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

// # TODO
// Consider the body of the object
int NextIsWall(struct Object *object, enum Direction direction) {
    struct Object virtual_object;
    virtual_object.body = object->body;
    virtual_object.left_x = object->left_x;
    virtual_object.top_y  = object->top_y;

    switch (direction) {
        case UP   : virtual_object.top_y  -= 1; break;
        case DOWN : virtual_object.top_y  += 1; break;
        case RIGHT: virtual_object.left_x += 1; break;
        case LEFT : virtual_object.left_x -= 1; break;
    }

    for (int i = 0; i < OBJECT_HEIGHT; i++) {
        for (int j = 0; j < OBJECT_WIDTH; j++) {
            if (virtual_object.body[i][j][0] != ' ') {
                int x = virtual_object.left_x + j;
                int y = virtual_object.top_y + i;

                if (map_data[y][x][0] == '@') {
                    return 1;
                }
            }
        }
    }

    return 0;
}

// # TODO
// the power_layzer of the character should be used.
#define LAYZER_POWER 50

// return 1 if character is dead
int getDamaged(struct CharacterManager *character_manager, int character_index) {
    character_manager->characters[character_index]->hp -= LAYZER_POWER;

    if (character_manager->characters[character_index]->hp < 1) {
        DeleteCharacter(character_manager, character_index);
        return 1;
    }

    return 0;
}

// # TODO
// Consier the body of the character

// The function is used by layzers
// This function is no longer used
/*
int NextIsCharacter(struct Layzer *layzer, enum Direction direction, struct CharacterManager *character_manager) {
    int layzer_x = layzer->object.x;
    int layzer_y = layzer->object.y;

    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {


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
}
*/

void CreateLayzer(struct Character *character) {
    for (int i = 0; i < MAX_LAYZERS; i++) {
        if (!character->layzer_manager.layzers_alive[i]) {
            character->layzer_manager.layzers[i] = malloc(sizeof(struct Layzer));
            character->layzer_manager.layzers_alive[i] = 1;

            // shortcut refe
            struct Layzer *layzerPtr = character->layzer_manager.layzers[i];

            // Initialize layzer
            layzerPtr->direction = character->direction;

            int x = character->object.x;
            int y = character->object.y;

            switch (character->direction) {
                case UP:
                    switch (character->layzerType) {
                        case GREENBEAM:
                            InitObject(&(layzerPtr->object), x, y-2, GreenBeam);
                            break;

                        case REDBEAM:
                            InitObject(&(layzerPtr->object), x, y-2, RedBeam);
                            break;
                    }
                    break;

                case DOWN:
                    switch (character->layzerType) {
                        case GREENBEAM:
                            InitObject(&(layzerPtr->object), x, y+2, GreenBeam);
                            break;

                        case REDBEAM:
                            InitObject(&(layzerPtr->object), x, y+2, RedBeam);
                            break;
                    }
                    break;
            }

            // # TODO
            // Caluculate leftX and topY using LAYZER_WIDTH and LAYZER_HEIGHT
            break;
        }
    }
}

void MovePlayer(struct Character *player, int *key_input, int pre_key_input) {
    switch (*key_input) {
        case 'w': case 'A':
            if (!NextIsWall(&player->object, UP)) {
                player->object.y -= 1;
                player->object.top_y -= 1;
            }
            break;

        case 's': case 'B':
            if (!NextIsWall(&player->object, DOWN)) {
                player->object.y += 1;
                player->object.top_y += 1;
            }
            break;

        case 'd': case 'C':
            if (!NextIsWall(&player->object, RIGHT)) {
                player->object.x += 1;
                player->object.left_x += 1;
            }
            break;

        case 'a': case 'D':
            if (!NextIsWall(&player->object, LEFT)) {
                player->object.x -= 1;
                player->object.left_x -= 1;
            }
            break;

        case ' ':
            CreateLayzer(player);

            // To preven stopping player when ' ' is typed
            // use previous input

            // This might cause a race condition because key_input is deferenced by another thread to get inputs
            // But.... This is not a big problem.
            *key_input = pre_key_input;
            MovePlayer(player, key_input, pre_key_input);
            break;
    }
}

// # TODO
// Have All enemies their own move patterns
void moveAllCharacters(struct CharacterManager *character_manager, int *key_input, int pre_key_input) {
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        // Filter player using direction. (only player have UP direction)
        if ((character_manager->character_alive[i] == 1) && (character_manager->characters[i]->direction == DOWN)) {
            // shortcut reference
            struct Character *characterPtr = character_manager->characters[i];

            switch (rand() % 6) {
                case 0 ... 2: // wait
                    break;

                case 3: // RIGHT
                    if (!NextIsWall(&characterPtr->object, RIGHT)) {
                        character_manager->characters[i]->object.x += 1;
                        character_manager->characters[i]->object.left_x += 1;
                    }
                    break;

                case 4: // LEFT
                    if (!NextIsWall(&characterPtr->object, LEFT)) {
                        character_manager->characters[i]->object.x -= 1;
                        character_manager->characters[i]->object.left_x -= 1;
                    }
                    break;

                case 5: // Layzer
                    CreateLayzer(characterPtr);
                    break;
            }
        } else if ((character_manager->character_alive[i] == 1) && (character_manager->characters[i]->direction == UP)) {
            MovePlayer(character_manager->characters[0], key_input, pre_key_input);
        }
    }
}

// This name reminds me ARP... I miss you.
// This function is no longer used!!
/*
struct Character *WhoIsAtXY(struct CharacterManager *character_manager, int x, int y) {
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            if ((character_manager->characters[i]->x == x) && (character_manager->characters[i]->y == y)) {
                return character_manager->characters[i];
            }
        }
    }
}
*/

void MoveAllLayzer(struct CharacterManager *character_manager) {
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
                            if (NextIsWall(&layzerPtr->object, layzerPtr->direction)) {
                                DeleteLayzer(&(characterPtr->layzer_manager), j);
                            } else {
                                layzerPtr->object.y -= 1;
                                layzerPtr->object.top_y -= 1;
                            }
                            break;

                        case DOWN:
                            if (NextIsWall(&layzerPtr->object, layzerPtr->direction)) {
                                DeleteLayzer(&(characterPtr->layzer_manager), j);
                            } else {
                                layzerPtr->object.y += 1;
                                layzerPtr->object.top_y += 1;
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

// What is this function?
void updateBuffer(struct InfoDisplay *infoDisplay, int score, int playerHp, char *msg) {
    infoDisplay->score = score;
    infoDisplay->playerHp = playerHp;
    infoDisplay->msg = msg;
}

// This function renders data before drawing.
void drawMapBuffer(struct CharacterManager *character_manager) {
    // create a buffer displayed.
    // All infomation, player, enemies, layzers..., is inserted into this buffer to draw.
    char buffer[MAP_HEIGHT][MAP_WIDTH][CHARS];

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            strcpy(buffer[i][j], map_data[i][j]);
        }
    }

    // insert chrarcter and layzers into buffer
    for (int i = 0; i < 100; i++) {
        if (character_manager->character_alive[i]) {
            // shortcut refe
            struct Character *characterPtr = character_manager->characters[i];
            int character_x = character_manager->characters[i]->object.x;
            int character_y = character_manager->characters[i]->object.y;

            // insert character
            for (int offsetY = 0; offsetY < OBJECT_HEIGHT; offsetY++) {
                for (int offsetX = 0; offsetX < OBJECT_WIDTH; offsetX++) {
                    if (characterPtr->object.body[offsetY][offsetX][0] != ' ') {
                        strcpy(buffer[characterPtr->object.top_y + offsetY][characterPtr->object.left_x + offsetX],
                               characterPtr->object.body[offsetY][offsetX]);
                    }
                }
            }

            // insert layzers
            for (int j = 0; j < MAX_LAYZERS; j++) {
                if (characterPtr->layzer_manager.layzers_alive[j]) {
                    // shortcut refe
                    struct Layzer *layzerPtr = characterPtr->layzer_manager.layzers[j];

                    for (int offsetY = 0; offsetY < OBJECT_HEIGHT; offsetY++) {
                        for (int offsetX = 0; offsetX < OBJECT_WIDTH; offsetX++) {
                            if (layzerPtr->object.body[offsetY][offsetX][0] != ' ') {
                                strcpy(buffer[layzerPtr->object.top_y + offsetY][layzerPtr->object.left_x + offsetX],
                                       layzerPtr->object.body[offsetY][offsetX]);
                            }
                        }
                    }
                }
            }
        }
    }

    // # TODO
    // convert 2 dimensional array into 1 dimensional before outputing into stdout
    // to reduce latency of drawing

    // draw buffer
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


// This function is used to get a input without Enter
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

void *InputThread (void *key_input) {
    while (1) {
        *(int *) key_input = getch();
    }

    return 0;
}

// if obj1 and obj2 are the same coordinates return true
int Collision(struct Object *obj1, struct Object *obj2) {
    for (int i = 0; i < OBJECT_HEIGHT; i++) {
        for (int j = 0; j < OBJECT_WIDTH; j++) {
            if (obj1->body[i][j][0] != ' ') {
                int obj1_x = obj1->left_x+j;
                int obj1_y = obj1->top_y+i;

                for (int k = 0; k < OBJECT_HEIGHT; k++) {
                    for (int l = 0; l < OBJECT_WIDTH; l++) {
                        if (obj2->body[k][l][0] != ' ') {
                            int obj2_x = obj2->left_x+l;
                            int obj2_y = obj2->top_y+k;

                            if (obj1_x == obj2_x && obj1_y == obj2_y) {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

// # TODO
// Consider body of object
// This function uses core coodinates for collision detection now

// detect all collisions of objects with layzers based on theri coordinates and body sizes.
// This function is neutral so there is no vector of collisions
void Collisions_char_lay(struct CharacterManager *character_manager) {
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if (character_manager->character_alive[i]) {
            // shortcut reference
            struct Character *characterPtr1 = character_manager->characters[i];

            for (int j = 0; j < MAX_CHARACTERS; j++) {
                if (character_manager->character_alive[j] && j != i) {
                    int dead = 0;

                    // shortcut reference
                    struct Character *characterPtr2 = character_manager->characters[j];

                    for (int k = 0; k < MAX_LAYZERS; k++) {
                        if (characterPtr2->layzer_manager.layzers_alive[k]) {
                            // shortcut reference
                            struct Layzer *layzerPtr = characterPtr2->layzer_manager.layzers[k];

                            if (Collision(&characterPtr1->object, &layzerPtr->object)) {
                                if (getDamaged(character_manager, i)) {
                                    dead++;
                                    break;
                                }
                            }
                        }
                    }

                    if (dead) break;
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

void TestGame() {
    struct CharacterManager character_manager;
    struct InfoDisplay infoDisplay;
    int key_input = 0;
    int pre_key_input = 0;
    int win = 0;
    int lose = 0;
    int done = 0;
    int score = 0;
    pthread_t thread_id;
    unsigned int msecounds = 100000;

    srand(time(NULL));
    pthread_create(&thread_id, NULL, InputThread, &key_input);

    InitInfoDisplay(&infoDisplay);
    InitCharacterManager(&character_manager);
    InitMapData();

    SpawnCharacter(&character_manager, 15, 25, UP, PLAYER, 100, GREENBEAM); // index 0 is the player
    SpawnCharacter(&character_manager, 4, 4, DOWN, KAI, 100, REDBEAM);
    //SpawnCharacter(&character_manager, 11, 5, DOWN, KAI, 100, REDBEAM);

    // shortcut refe
    struct Character *player = character_manager.characters[0];

    // Collision detection is peformed after all character and layzers moved
    while (!win && !lose) {
        MoveAllLayzer(&character_manager);
        moveAllCharacters(&character_manager, &key_input, pre_key_input);

        Collisions_char_lay(&character_manager);

        UpdateInfoDisplay(&infoDisplay, score, player->hp, NULL);
        draw(&infoDisplay, &character_manager);

        if (!character_manager.character_alive[0]) {
            printf("----- GAME OVER -----\n");
            lose = 1;
        }

        for (int i = 1; i < MAX_CHARACTERS; i++) {
            if (character_manager.character_alive[i]) {
                break;
            } else if (i == MAX_CHARACTERS - 1) {
                printf("----- VICTORY -----\n");
                win = 1;
            }
        }

        pre_key_input = key_input;
        usleep(msecounds);
    }

    pthread_cancel(thread_id);

    return;
}

int main() {
    // I think managing user input using a integer is better, but somehow scanf("%d", &user_input) makes a loop bug.
    char user_input[100];
    int quit = 0;

    printf("\x1B[92m");
    printf("         __                __\n");
    printf("   _____/ /_  ____  ____  / /____  _____\n");
    printf("  / ___/ __ \\/ __ \\/ __ \\/ __/ _ \\/ ___/\n");
    printf(" (__  ) / / / /_/ / /_/ / /_/  __/ /\n");
    printf("/____/_/ /_/\\____/\\____/\\__/\\___/_/\n");
    printf("\x1B[0m");
    printf("\n\n\n");

    while (!quit) {
        // display menu
        printf("------ MODE -------\n");
        printf("1: Test Gmae\n");
        printf("2: ...\n");
        printf("3: ...");
        printf("4: Quit\n");

        printf("\x1b[32;40m>\x1b[37;40m");
        scanf("%s", user_input);
        printf("\n");

        switch (user_input[0]) {
            case '1': TestGame(); break;
            case '4': quit++; break;
            default: 
                printf("Invalid Input\n");
        }
    }
    return 0;
}
