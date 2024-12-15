#pragma once
#include "Character.h"
#include <SFML/Graphics.hpp>

// Represents Captain America, derived from Character
class CaptainAmerica : public Character {
public:
    // Constructor initializes Captain America with specific stats
    CaptainAmerica() : Character("Captain America", 140, 12, 5) {}
    
    // Override attack method
    void attack(Character& target) override;
    
    // Override defend method
    void defend() override;
    
    // Override special method
    void special(Character& target) override;
    
    // Override draw method to render Captain America
    void draw(sf::RenderWindow& window, sf::Vector2f position) override;  // Add draw method declaration
};

