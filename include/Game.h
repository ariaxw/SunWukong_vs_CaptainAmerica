#ifndef GAME_H
#define GAME_H

#include <thread>
#include <mutex>
#include <atomic>

#include <SFML/Graphics.hpp>
#include "Character.h"
#include "SunWukong.h"
#include "CaptainAmerica.h"

// Manages the game logic and rendering
class Game {
public:
    Game(int playerChoice);
    ~Game();
    void run();
    void showMainMenu();
    void showCharacterSelection();
    int currentPlayerChoice;


private:
    // Core game mechanics
    void handleInput();
    void updateAnimations();
    void opponentAction();
    void render();
    void animateAction(Character* character, const std::string& actionType);
    void showRules();
    void showRoundResult(const std::string& result);
    void showRoundEndOptions(const std::string& result); 
    void showFinalResultUI();
    void resetGame();
    void resetGame(int playerChoice);
    void updateDamageTexts();
    
    // Animation and particle management
    std::thread animationThread; // animation thread
    std::atomic<bool> stopThread;
    std::mutex renderMutex;
    
    sf::Clock actionClock;
    void updateActionStatusEffect();
    
    sf::VertexArray particles; // Store particles
    std::vector<sf::Vector2f> particleVelocities; // Store particle velocities
    sf::Clock particleClock; // Timer for particle updates
    void updateParticles(); // Update particle positions
    //void generateParticles(sf::Vector2f position, int count, sf::Color color);
    void generateParticlesRecursive(sf::Vector2f position, int count, sf::Color color);
    void generateParticles(sf::Vector2f position, int count, sf::Color color);

    // Game state variables
    sf::RenderWindow window;
    Character* player;
    Character* opponent;
    bool isPlayerTurn;

    // Game delay management
    bool isDelayed;
    sf::Clock delayClock;

    // Textures and sprites for player and opponent attributes
    sf::Texture playerTexture;
    sf::Texture opponentTexture;
    sf::Sprite playerSprite;
    sf::Sprite opponentSprite;

    // Font and text for displaying parameters
    sf::Font font;
    sf::Text titleText;
    sf::Text playerHealthText;
    sf::Text opponentHealthText;
    sf::Text playerSpecialText;
    sf::Text opponentSpecialText;
    // Texts for rules, action status, and damage/defense
    sf::Text rulesText;
    sf::Text actionStatusText;
    sf::Text playerDamageText;  // Added for displaying player damage/defense text
    sf::Text opponentDamageText;  // Added for displaying opponent damage/defense text

    // Status bars
    sf::RectangleShape playerHealthBar;
    sf::RectangleShape opponentHealthBar;
    sf::RectangleShape playerSpecialBar;
    sf::RectangleShape opponentSpecialBar;

    // Grouped frames
    sf::RectangleShape playerFrame;
    sf::RectangleShape opponentFrame;
    
    // Background texture and sprite
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // Game progress variables
    int playerWins = 0;
    int opponentWins = 0;
    int currentRound = 1;
    static const int maxRounds = 3;

    void resetCharacters(); // Reset characters for the next round
    void nextRound(); // Proceed to the next round
    void displayFinalResults(); // Show results at the end of the game
};

#endif // GAME_H
