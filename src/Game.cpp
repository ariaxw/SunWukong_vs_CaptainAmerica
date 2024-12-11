#include "Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <regex>

Game::Game(int playerChoice)
    : stopThread(false),
      window(sf::VideoMode(1100, 820), "Sun Wukong vs Captain America"),
      isPlayerTurn(true) {
          
    if (playerChoice == 1) {
        player = new SunWukong();
        opponent = new CaptainAmerica();
        if (!playerTexture.loadFromFile("../assets/sunwukong.png") ||
            !opponentTexture.loadFromFile("../assets/captainamerica.png")) {
            throw std::runtime_error("Failed to load images!");
        }
    } else if (playerChoice == 2) {
        player = new CaptainAmerica();
        opponent = new SunWukong();
        if (!playerTexture.loadFromFile("../assets/captainamerica.png") ||
            !opponentTexture.loadFromFile("../assets/sunwukong.png")) {
            throw std::runtime_error("Failed to load images!");
        }
    }
    
    playerSprite.setTexture(playerTexture);
    opponentSprite.setTexture(opponentTexture);
    playerSprite.setPosition(65, 480); // Player stays on the left
    opponentSprite.setPosition(650, 480);
    
    if (!font.loadFromFile("../assets/arial.ttf")) {
        throw std::runtime_error("Failed to load font!");
    }
    
    rulesText.setFont(font);
    rulesText.setCharacterSize(30);
    rulesText.setPosition(200, 150);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setString(R"(
Rules: 

A: Attack - Deals damage 
   <Colorful Particle Effect>

D: Defend - Reduces damage 
   <Shield Effect>

S: Special Attack - High damage 
   <Colorful Spirit Flash and Particle Effect>

Press [Enter] to start! Gogogo!
)");
    
    // Title Text
    titleText.setFont(font);
    titleText.setCharacterSize(30);
    titleText.setPosition(200, 15);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Epic Battle: Sun Wukong vs. Captain America");
    
    // Player and opponent grouped frames
    playerFrame.setSize(sf::Vector2f(350, 300));
    playerFrame.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent background
    playerFrame.setOutlineThickness(2);
    playerFrame.setOutlineColor(sf::Color::White);
    playerFrame.setPosition(50, 80);
    
    opponentFrame.setSize(sf::Vector2f(350, 300));
    opponentFrame.setFillColor(sf::Color(50, 50, 50, 200));
    opponentFrame.setOutlineThickness(2);
    opponentFrame.setOutlineColor(sf::Color::White);
    opponentFrame.setPosition(620, 80);
    
    // Health bars
    playerHealthBar.setSize(sf::Vector2f(250, 20));
    playerHealthBar.setFillColor(sf::Color::Green);
    playerHealthBar.setPosition(80, 100);
    
    opponentHealthBar.setSize(sf::Vector2f(250, 20));
    opponentHealthBar.setFillColor(sf::Color::Green);
    opponentHealthBar.setPosition(650, 100);
    
    // Special bars
    playerSpecialBar.setSize(sf::Vector2f(250, 10));
    playerSpecialBar.setFillColor(sf::Color::Blue);
    playerSpecialBar.setPosition(80, 170);
    
    opponentSpecialBar.setSize(sf::Vector2f(250, 10));
    opponentSpecialBar.setFillColor(sf::Color::Blue);
    opponentSpecialBar.setPosition(650, 170);
    
    // Text settings
    playerHealthText.setFont(font);
    playerHealthText.setCharacterSize(18);
    playerHealthText.setFillColor(sf::Color::White);
    playerHealthText.setPosition(80, 130);
    
    opponentHealthText.setFont(font);
    opponentHealthText.setCharacterSize(18);
    opponentHealthText.setFillColor(sf::Color::White);
    opponentHealthText.setPosition(650, 130);
    
    playerSpecialText.setFont(font);
    playerSpecialText.setCharacterSize(18);
    playerSpecialText.setFillColor(sf::Color::Cyan);
    playerSpecialText.setPosition(80, 190);
    
    opponentSpecialText.setFont(font);
    opponentSpecialText.setCharacterSize(18);
    opponentSpecialText.setFillColor(sf::Color::Cyan);
    opponentSpecialText.setPosition(650, 190);
    
    playerDamageText.setFont(font);
    playerDamageText.setCharacterSize(15);
    playerDamageText.setFillColor(sf::Color::Red);
    playerDamageText.setPosition(80, 260);
    
    opponentDamageText.setFont(font);
    opponentDamageText.setCharacterSize(15);
    opponentDamageText.setFillColor(sf::Color::Red);
    opponentDamageText.setPosition(650, 260);

    // Action status
    actionStatusText.setFont(font);
    actionStatusText.setCharacterSize(25);
    actionStatusText.setFillColor(sf::Color::Yellow);
    actionStatusText.setPosition(150, 420);
            
    particles.setPrimitiveType(sf::Points); // Use points for particles
          
    animationThread = std::thread(&Game::updateAnimations, this);
}

Game::~Game() {
    stopThread = true;
    if (animationThread.joinable()) {
        animationThread.join();
    }
}

void Game::run() {
    showRules();
    while (window.isOpen() && currentRound <= maxRounds) {
        while (player->isAlive() && opponent->isAlive()) {
            handleInput(); // Handle player input, including checking for exit
            
            if (!window.isOpen()) {
                 break; // Exit the loop if the window is closed
             }
            
            if (isDelayed) {
                // check delay
                if (delayClock.getElapsedTime().asMilliseconds() >= 800) { // delay 1 sec
                    isDelayed = false;
                    opponentAction();
                    isPlayerTurn = true; // new turn for player
                }
            } else if (!isPlayerTurn && opponent->isAlive()) {
                // set delay condition
                isDelayed = true;
                delayClock.restart();
            }
            render();
        }

        if (!window.isOpen()) {
            break; // Exit the main loop if the window is closed
        }

        std::string roundResult;
        if (player->isAlive()) {
            playerWins++;
            roundResult = "Player Wins this Round! " + std::to_string(currentRound) + "!";
        } else {
            opponentWins++;
            roundResult = "Opponent Wins this Round! " + std::to_string(currentRound) + "!";
        }

        showRoundEndOptions(roundResult);
        
        if (!window.isOpen()) {
            break; // Exit the main loop if the window is closed during round-end options
        }
        
        // Show final result after three rounds
        if (currentRound >= maxRounds) {
            showFinalResultUI();
            window.close(); // close game window
        } else {
            nextRound(); // go into next round
        }
    }
}

template <typename T>
T interpolate(const T& start, const T& end, float t) {
    return start + t * (end - start);
}
sf::Color interpolateColor(const sf::Color& start, const sf::Color& end, float t) {
    return sf::Color(
        static_cast<sf::Uint8>(interpolate(start.r, end.r, t)),
        static_cast<sf::Uint8>(interpolate(start.g, end.g, t)),
        static_cast<sf::Uint8>(interpolate(start.b, end.b, t)),
        255
    );
}

void Game::updateActionStatusEffect() {
    static std::vector<sf::Color> colors = {
        sf::Color::Red,
        sf::Color::Yellow,
        sf::Color(255, 165, 0), // Orange
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Magenta,
        sf::Color::Cyan
    };

    float elapsedTime = actionClock.getElapsedTime().asSeconds();
    int currentIndex = static_cast<int>(elapsedTime * 3) % colors.size();
    int nextIndex = (currentIndex + 1) % colors.size();

    float t = elapsedTime - static_cast<int>(elapsedTime);
    sf::Color currentColor = interpolateColor(colors[currentIndex], colors[nextIndex], t);
    actionStatusText.setFillColor(currentColor);
}


void Game::resetCharacters() {
    // Determine the player's character type
    Character* tempPlayer = nullptr;
    Character* tempOpponent = nullptr;

    if (typeid(*player) == typeid(SunWukong)) {
        tempPlayer = new SunWukong();
    } else if (typeid(*player) == typeid(CaptainAmerica)) {
        tempPlayer = new CaptainAmerica();
    }

    // Determine the opponent's character type
    if (typeid(*opponent) == typeid(SunWukong)) {
        tempOpponent = new SunWukong();
    } else if (typeid(*opponent) == typeid(CaptainAmerica)) {
        tempOpponent = new CaptainAmerica();
    }

    // Cleanup old characters
    delete player;
    delete opponent;

    // Assign new characters
    player = tempPlayer;
    opponent = tempOpponent;
    isPlayerTurn = true;
}


void Game::nextRound() {
    if (currentRound < maxRounds) {
        currentRound++;
        resetCharacters();
        player->resetCriticalHit();
        opponent->resetCriticalHit();
    } else {
        displayFinalResults();
        window.close();
    }
}

void Game::displayFinalResults() {
    std::cout << "Game Over!\n";
    std::cout << "Player Wins: " << playerWins << "\n";
    std::cout << "Opponent Wins: " << opponentWins << "\n";

    if (playerWins > opponentWins) {
        std::cout << "Congratulations! You are the champion!\n" << std::endl;
    } else if (opponentWins > playerWins) {
        std::cout << "Better luck next time! Opponent wins!\n" << std::endl;
    } else {
        std::cout << "It's a tie! Well played!\n" << std::endl;
    }
}

void Game::showRoundResult(const std::string& result) {
    sf::Text roundResultText;
    roundResultText.setFont(font);
    roundResultText.setCharacterSize(30);
    roundResultText.setFillColor(sf::Color::Yellow);
    roundResultText.setPosition(280, 150);
    roundResultText.setString(result);

    window.clear();
    render();  // Render all current game elements
    window.draw(roundResultText);
    window.display();

    sf::sleep(sf::seconds(3));  // Pause for 3 seconds to show the result
}

void Game::showMainMenu() {
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(30);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(100, 180);
    menuText.setString("------------------------------------------------------------------------------------------\n"
                       "Welcome to the Epic Battle: Sun Wukong vs. Captain America!\n"
                       "\n"
                       "1. Start Game! Wakaka!  \n"
                       "2. Exit Game...See You Next Time  \n"
                       "\n"
                       "Press [1] to Start or [2] to Exit. \n"
                       "------------------------------------------------------------------------------------------");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    showCharacterSelection(); // Proceed to character selection
                    return;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    window.close(); // Exit game
                    return;
                }
            }
        }
        window.clear();
        window.draw(menuText);
        window.display();
    }
}

void Game::showCharacterSelection() {
    sf::Text selectionText;
    selectionText.setFont(font);
    selectionText.setCharacterSize(30);
    selectionText.setFillColor(sf::Color::White);
    selectionText.setPosition(150, 180);
    selectionText.setString("------------------------------------------\n"
                            "Choose your character:\n"
                            "\n"
                            "1. Sun Wukong\n"
                            "2. Captain America\n"
                            "\n"
                            "Press [1] or [2].\n"
                            "------------------------------------------\n");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    currentPlayerChoice = 1;
                    resetGame(1); // Pass Sun Wukong as player
                    return;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    currentPlayerChoice = 2;
                    resetGame(2); // Pass Captain America as player
                    return;
                }
            }
        }
        window.clear();
        window.draw(selectionText);
        window.display();
    }
}

void Game::showRules() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                return;
            }
        }
        window.clear();
        window.draw(rulesText);
        window.display();
    }
}

void Game::showRoundEndOptions(const std::string& result) {
    sf::Text roundEndText;
    roundEndText.setFont(font);
    roundEndText.setCharacterSize(30);
    roundEndText.setFillColor(sf::Color::White);
    roundEndText.setPosition(150, 250);
    roundEndText.setString("----------------------------------------------------------------------\n" + result +
                           "\nPress [Enter] to continue to next round or [Esc] to quit.\n"
                           "----------------------------------------------------------------------\n");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    return; // continue next round
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close(); // exit game
                    return;
                }
            }
        }
        window.clear(sf::Color::Black);
        window.draw(roundEndText);
        //render();
        window.display();
        sf::sleep(sf::milliseconds(50));
    }
}

void Game::showFinalResultUI() {
    std::string finalResult = "Game Over!\n";
    finalResult += "\nPlayer Wins: " + std::to_string(playerWins) + "\n";
    finalResult += "\nOpponent Wins: " + std::to_string(opponentWins) + "\n";

    if (playerWins > opponentWins) {
        finalResult += "\nCongratulations! You are the champion!";
    } else if (opponentWins > playerWins) {
        finalResult += "\nBetter luck next time! Opponent wins!";
    } else {
        finalResult += "It's a tie! Well played!";
    }

    finalResult += "\n--------------------------------------------------------------\n"
                   "\nPress [Enter] to start a new game or [Esc] to quit.\n"
                   "\n--------------------------------------------------------------\n";
    
    sf::Text finalResultText;
    finalResultText.setFont(font);
    finalResultText.setCharacterSize(30);
    finalResultText.setFillColor(sf::Color::Yellow);
    finalResultText.setPosition(150, 200);
    finalResultText.setString(finalResult);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    resetGame(); // reset game to start again
                    return;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    window.close(); // exit game
                    return;
                }
            }
        }
        window.clear();
        window.draw(finalResultText);
        window.display();
    }
}

void Game::resetGame() {
    if (player) delete player;
    if (opponent) delete opponent;

    currentRound = 1;
    playerWins = 0;
    opponentWins = 0;
    resetGame(currentPlayerChoice);
}

void Game::resetGame(int playerChoice) {
    if (player) delete player;
    if (opponent) delete opponent;

    if (playerChoice == 1) {
        player = new SunWukong();
        opponent = new CaptainAmerica();
    } else if (playerChoice == 2) {
        player = new CaptainAmerica();
        opponent = new SunWukong();
    }

    if (!playerTexture.loadFromFile(playerChoice == 1 ? "../assets/sunwukong.png" : "../assets/captainamerica.png")) {
        std::cerr << "Failed to load player texture in resetGame!" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!opponentTexture.loadFromFile(playerChoice == 1 ? "../assets/captainamerica.png" : "../assets/sunwukong.png")) {
        std::cerr << "Failed to load opponent texture in resetGame!" << std::endl;
        exit(EXIT_FAILURE);
    }

    playerSprite.setTexture(playerTexture);
    opponentSprite.setTexture(opponentTexture);
    isPlayerTurn = true;
}


void Game::handleInput() {
    sf::Event event;
    //main loop
    while (window.pollEvent(event)) {
        // Close the window when the user clicks the close button
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Exit the game when the user presses the Escape key
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
        
        // Handle TextEntered for regex validation
        if (event.type == sf::Event::TextEntered) {
            static const std::regex actionRegex("^[ADSads]$"); // only execute A/D/S command
            char inputChar = static_cast<char>(event.text.unicode);
            
            // Validate input //Regular Expression
            if (!std::regex_match(std::string(1, inputChar), actionRegex)) {
                std::cout << "Invalid action! Please enter A, D, or S.\n";
            } else {
                std::cout << "Valid action: " << inputChar << std::endl;
            }
        }
        
        // Player's turn actions
        if (isPlayerTurn && event.type == sf::Event::KeyPressed) {
            std::map<sf::Keyboard::Key, std::function<void()>> actions = {
                {sf::Keyboard::A, [&]() {
                    player->attack(*opponent);
                    actionStatusText.setString("Player attacked and caused " +
                                               std::to_string(player->getLastDamage()) +
                                               (player->isCriticalHit() ? " (Critical!)" : "") +
                                               " damage.");
                    updateDamageTexts();
                    player->decreaseCooldown();
                    opponent->decreaseCooldown();
                    animateAction(player, "attack");
                    isPlayerTurn = false;
                }},
                
                {sf::Keyboard::D, [&]() {
                    player->defend();
                    actionStatusText.setString("Player defended, blocked " +
                                               std::to_string(player->getBlockedDamage()) +
                                               " damage, and recovered "+
                                               std::to_string(player->getLastRecoveredHealth()) + " health!"
                                               );
                    updateDamageTexts();
                    player->decreaseCooldown();
                    opponent->decreaseCooldown();
                    animateAction(player, "defend");
                    render();  // Ensure render happens immediately
                    sf::sleep(sf::milliseconds(500));  // Add delay for visibility
                    isPlayerTurn = false;
                }},
                
                {sf::Keyboard::S, [&]() {
                    if (player->canUseSpecial()) {
                        player->special(*opponent);
                        player->resetCooldown(5); // Reset cooldown
                        actionStatusText.setString("Player used a special attack and caused " +
                                                   std::to_string(player->getExtraDamage()) + " damage!");
                    } else {
                        //actionStatusText.setString("Player tried a special attack but it's not available. Check cooldown or special points.");
                        std::string errorMsg;
                        if (player->getCooldownTimer() > 0) {
                            errorMsg = "Player tried to use a special attack, but it's on cooldown for " +
                            std::to_string(player->getCooldownTimer()) + " more turns!";
                        } else if (player->getSpecialPoints() < 20) {
                            errorMsg = "Player tried to use a special attack, but lacked enough special points! (" +
                            std::to_string(player->getSpecialPoints()) + "/20)";
                        }
                        actionStatusText.setString(errorMsg);
                    }
                    updateDamageTexts();
                    player->decreaseCooldown();
                    opponent->decreaseCooldown();
                    animateAction(player, "special");
                    isPlayerTurn = false;
                }}
            };
            if (actions.find(event.key.code) != actions.end()) {
                actions[event.key.code]();
            }
        }
    }
}

void Game::opponentAction() {
    int actionType = rand() % 3; // Randomly choose the opponent's action
    if (actionType == 0) {
        opponent->attack(*player);
        actionStatusText.setString("Opponent attacked and caused " +
                                   std::to_string(opponent->getLastDamage()) +
                                   (opponent->isCriticalHit() ? " (Critical!)" : "") +
                                   " damage.");
        
    } else if (actionType == 1) {
        opponent->defend();
        actionStatusText.setString("Opponent defended, blocked " +
                                   std::to_string(opponent->getBlockedDamage()) +
                                   " damage, and recovered " +
                                   std::to_string(opponent->getLastRecoveredHealth()) + " health!");
        animateAction(opponent, "defend");
        
    } else if (actionType == 2) {
        if (opponent->canUseSpecial()) {
            opponent->special(*player);
            opponent->resetCooldown(5); // Reset cooldown
            actionStatusText.setString("Opponent used a special attack and caused " +
                                       std::to_string(opponent->getExtraDamage()) + " damage!");
        } else {
            std::string errorMsg;
            if (opponent->getCooldownTimer() > 0) {
                errorMsg = "Opponent tried to use a special attack, but it's on cooldown for " +
                std::to_string(opponent->getCooldownTimer()) + " more turns!";
            } else if (opponent->getSpecialPoints() < 20) {
                errorMsg = "Opponent tried to use a special attack, but lacked enough special points! (" +
                std::to_string(opponent->getSpecialPoints()) + "/20)";
            }
            actionStatusText.setString(errorMsg);
        }
    }
    updateDamageTexts();
    animateAction(opponent, (actionType == 2) ? "special" : "attack");
    player->decreaseCooldown();
    opponent->decreaseCooldown();
}


void Game::animateAction(Character* character, const std::string& actionType) {
    sf::Sprite* sprite = (character == player) ? &playerSprite : &opponentSprite;
    sf::Vector2f originalPosition = sprite->getPosition(); // Save original position

    if (actionType == "defend") {
        // Load the shield texture
        sf::Texture shieldTexture;
        if (!shieldTexture.loadFromFile("../assets/shield.png")) { // Adjust path if needed
            std::cerr << "Failed to load shield texture!" << std::endl;
            return;
        }
        // Create the shield sprite
        sf::Sprite shieldSprite(shieldTexture);
        shieldSprite.setOrigin(shieldTexture.getSize().x / 2.f, shieldTexture.getSize().y / 2.f); // Center the shield
        shieldSprite.setPosition(sprite->getPosition().x + sprite->getGlobalBounds().width / 2,
                                 sprite->getPosition().y + sprite->getGlobalBounds().height / 2);

        // Animate the shield with a slight up-and-down movement
        for (int i = 0; i < 5; ++i) {
            shieldSprite.move(0, (i % 2 == 0) ? -5.f : 5.f); // Move up and down alternately
            window.clear();
            render(); // Render existing game elements
            window.draw(shieldSprite); // Draw the shield on top
            window.display();
            sf::sleep(sf::milliseconds(100)); // Pause briefly for visibility
        }
        
    } else if (actionType == "special") {
        // Flash the sprite with multiple colors for a special attack
        sf::Color colors[] = {sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Magenta};
        for (int i = 0; i < 10; ++i) {
            sprite->setColor(colors[i % 5]); // Cycle through colors
            sprite->move((i % 2 == 0) ? 5.f : -5.f, 0.f); // Slightly move right and left
            sf::Vector2f centerPosition = sprite->getPosition() + sf::Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
            generateParticles(centerPosition, 650, colors[i % 5]); // Dynamic color particles
            render();
            sf::sleep(sf::milliseconds(100)); // Pause briefly for effect
        }
        sprite->setColor(sf::Color::White); // Reset to default color
        
    } else if (actionType == "attack") {
        // Perform a simple attack animation with movement
        sf::Color colors[] = {sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Magenta};
        for (int i = 0; i < 3; ++i) {
            sprite->move(15.f, 0.f); // Move to the right
            render();
            sf::sleep(sf::milliseconds(100));
            sf::Vector2f centerPosition = sprite->getPosition() + sf::Vector2f(sprite->getGlobalBounds().width / 2, sprite->getGlobalBounds().height / 2);
            generateParticles(centerPosition, 650, colors[i % 5]);
            sprite->move(-15.f, 0.f); // Return to original position
            render();
            sf::sleep(sf::milliseconds(100));
        }
    }

    sprite->setPosition(originalPosition); // Restore the sprite's original position
}

void Game::generateParticlesRecursive(sf::Vector2f position, int count, sf::Color color) {
    
    static const std::vector<sf::Color> colors = {
               sf::Color::Red, sf::Color::Yellow, sf::Color::Green, sf::Color::Blue, sf::Color::Magenta
           };

    if (count <= 0) return; // Recursive end condition

    sf::Vertex particle;
    float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f; // Random angle in radians
    float radius = static_cast<float>(rand() % 130 + 100); // Random radius
    sf::Vector2f offset(radius * cos(angle), radius * sin(angle)); // Convert polar to Cartesian

    particle.position = position + offset;
    particle.color = color;

    particles.append(particle);
    particleVelocities.push_back(offset * 0.1f); // Adjust speed

    // Recursive Run
    generateParticlesRecursive(position, count - 1, color);
}

void Game::generateParticles(sf::Vector2f position, int count, sf::Color color) {
    particles.clear();
    particleVelocities.clear();
    generateParticlesRecursive(position, count, color);
}

void Game::updateParticles() {
    for (size_t i = 0; i < particleVelocities.size(); ++i) {
        particles[i].position += particleVelocities[i];
    }
    particleClock.restart();
}

void Game::updateDamageTexts() {
    playerDamageText.setString(
        "- Last Damage: " + std::to_string(player->getLastDamage()) + "\n" +
        "- Critical Hit: " + (player->isCriticalHit() ? "Yes" : "No") + "\n" +
        "- Last Critical Damage: " + std::to_string(player->isCriticalHit() ? player->getCriticalDamage() : 0) + "\n" +
        "- Last Blocked Damage: " + std::to_string(player->getBlockedDamage()) + "\n" +
        "- Last Special Recovered: " + std::to_string(player->getSpecialRecovered()) + "\n" +
        "- Last Health Recovered: " + std::to_string(player->getLastRecoveredHealth()) + "\n"
    );

    opponentDamageText.setString(
        "- Last Damage: " + std::to_string(opponent->getLastDamage()) + "\n" +
        "- Critical Hit: " + (opponent->isCriticalHit() ? "Yes" : "No") + "\n" +
        "- Last Critical Damage: " + std::to_string(opponent->isCriticalHit() ? opponent->getCriticalDamage() : 0) + "\n"+
        "- Last Blocked Damage: " + std::to_string(opponent->getBlockedDamage()) + "\n" +
        "- Last Special Recovered: " + std::to_string(opponent->getSpecialRecovered()) + "\n" +
        "- Last Health Recovered: " + std::to_string(opponent->getLastRecoveredHealth()) + "\n"
    );
}

void Game::updateAnimations() {
    while (!stopThread) {
        {
            std::lock_guard<std::mutex> lock(renderMutex); // lock mutex
            updateParticles();
            updateActionStatusEffect();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::render() {
    std::lock_guard<std::mutex> lock(renderMutex); //lock mutex
    
    window.clear();

    // Update health bars
    playerHealthBar.setSize(sf::Vector2f(player->getHealth() * 2, 20));
    opponentHealthBar.setSize(sf::Vector2f(opponent->getHealth() * 2, 20));

    // Update special bars
    playerSpecialBar.setSize(sf::Vector2f(player->getSpecialPoints() * 2, 10));
    opponentSpecialBar.setSize(sf::Vector2f(opponent->getSpecialPoints() * 2, 10));
    
    // Update texts
    playerHealthText.setString("Health: " + std::to_string(player->getHealth()));
    opponentHealthText.setString("Health: " + std::to_string(opponent->getHealth()));

    playerSpecialText.setString("Special: " + std::to_string(player->getSpecialPoints()));
    opponentSpecialText.setString("Special: " + std::to_string(opponent->getSpecialPoints()));
    
    // Add cooldown display for player
    sf::Text playerCooldownText;
    playerCooldownText.setFont(font);
    playerCooldownText.setCharacterSize(18);
    playerCooldownText.setFillColor(sf::Color::Cyan);
    playerCooldownText.setPosition(80, 220);
    playerCooldownText.setString("Cooldown: " + std::to_string(player->getCooldownTimer()));

    // Add cooldown display for opponent
    sf::Text opponentCooldownText;
    opponentCooldownText.setFont(font);
    opponentCooldownText.setCharacterSize(18);
    opponentCooldownText.setFillColor(sf::Color::Cyan);
    opponentCooldownText.setPosition(650, 220);
    opponentCooldownText.setString("Cooldown: " + std::to_string(opponent->getCooldownTimer()));
    
    // Draw background
    if (backgroundSprite.getTexture()) {
        window.draw(backgroundSprite);
    }
    
    // Draw title
    window.draw(titleText);
    
    // Draw grouped frames
    window.draw(playerFrame);
    window.draw(opponentFrame);
    
    // Draw elements
    window.draw(playerHealthBar);
    window.draw(opponentHealthBar);
    window.draw(playerSpecialBar);
    window.draw(opponentSpecialBar);

    window.draw(playerHealthText);
    window.draw(opponentHealthText);
    window.draw(playerSpecialText);
    window.draw(opponentSpecialText);

    updateActionStatusEffect();
    window.draw(actionStatusText);
    window.draw(playerDamageText);
    window.draw(opponentDamageText);

    window.draw(playerSprite);
    window.draw(opponentSprite);
    
    window.draw(playerCooldownText);
    window.draw(opponentCooldownText);

    // Draw any particle effects
    window.draw(particles);
    
    window.display();
}
