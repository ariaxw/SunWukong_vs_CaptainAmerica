#pragma once
#include "Character.h"

// Represents Sun Wukong, derived from Character
class SunWukong : public Character {
public:
    // Constructor initializes Sun Wukong with specific stats
    SunWukong() : Character("Sun Wukong", 150, 10, 3) {}

    // Override attack method
    void attack(Character& target) override;
    
    // Override defend method
    void defend() override;
    
    // Override special method
    void special(Character& target) override;
    
    void draw(sf::RenderWindow& window, sf::Vector2f position) override;  // Add draw method
};

