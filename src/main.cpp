#include "Game.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>


int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    
    try {
        Game game(1); // Initialize the game with a default character
        game.showMainMenu(); // Display the main menu UI
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
