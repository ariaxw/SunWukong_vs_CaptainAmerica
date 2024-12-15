#include "Game.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>

// Entry point of the application
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    try {
        Game game(1); // Initialize the game with a default character
        game.showMainMenu(); // Display the main menu UI
        game.run(); // Start the game loop
    } catch (const std::exception& e) {
        // Handle any critical errors that occur during execution
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return -1; // Return error code
    }
    return 0;
}
