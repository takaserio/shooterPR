#include <iostream>
#include <chrono>
#include <thread>

#include "input.cpp"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "EnemyManager.h"
#include "body_data"

using namespace std;

int main() {
    int key_input;
    std::thread *thread_ptr = keyInputThread(&key_input);
    
    Player player;
    Map map(0);
    EnemyManager enemy_manager;

    enemy_manager.spawn(15, 20, NORMAL);

    while (1) {
        if (key_input  != 0) {
            cout << key_input << "\n";
            key_input = 0;
        }
        player.move(key_input, map.getBuffer());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete thread_ptr;
}