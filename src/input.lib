#include <thread>
#include <termios.h>
#include <unistd.h>
#include <iostream>

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void input_loop(int *key_input) {
    while (1) {
        *key_input = getch();
    }
}

std::thread *keyInputThread(int *key_input) {
    return new std::thread(input_loop, key_input);
}