#pragma once
#include "Character.h"
#include <SFML/Graphics.hpp>

class CaptainAmerica : public Character {
public:
    CaptainAmerica() : Character("Captain America", 140, 10, 5) {}
    
    void attack(Character& target) override;
    void defend() override;
    void special(Character& target) override;
    void draw(sf::RenderWindow& window, sf::Vector2f position) override;  // Add draw method declaration
};

