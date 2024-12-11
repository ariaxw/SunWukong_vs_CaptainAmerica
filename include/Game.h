#ifndef GAME_H
#define GAME_H

#include <thread>
#include <mutex>
#include <atomic>

#include <SFML/Graphics.hpp>
#include "Character.h"
#include "SunWukong.h"
#include "CaptainAmerica.h"

class Game {
public:
    Game(int playerChoice);
    ~Game();
    void run();
    void showMainMenu();
    void showCharacterSelection();
    int currentPlayerChoice;


private:
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

    sf::RenderWindow window;
    Character* player;
    Character* opponent;
    bool isPlayerTurn;

    bool isDelayed;
    sf::Clock delayClock;

    // Textures and sprites for characters
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
    
    int playerWins = 0;
    int opponentWins = 0;
    int currentRound = 1;
    static const int maxRounds = 3;

    void resetCharacters();
    void nextRound();
    void displayFinalResults();
};

#endif // GAME_H
