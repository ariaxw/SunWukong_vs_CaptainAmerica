#pragma once
#include "Character.h"

class SunWukong : public Character {
public:
    SunWukong() : Character("Sun Wukong", 150, 12, 3) {}

    void attack(Character& target) override;
    void defend() override;  // Implement defend method
    void special(Character& target) override;  // Implement special method
    void draw(sf::RenderWindow& window, sf::Vector2f position) override;  // Add draw method
};

