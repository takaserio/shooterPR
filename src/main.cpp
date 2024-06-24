#include <iostream>
#include <chrono>
#include <thread>

#include "input.cpp"

using namespace std;

int main() {
    int key_input;
    std::thread *thread_ptr = keyInputThread(&key_input);

    while (1) {
        if (key_input  != 0) {
            cout << key_input << "\n";
            key_input = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete thread_ptr;
}