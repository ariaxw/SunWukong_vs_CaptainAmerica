#include "SunWukong.h"
#include <iostream>
#include <filesystem>

// Sun Wukong to perform a basic attack
void SunWukong::attack(Character& target) {
    applyCriticalHit(); // Calculate if the attack is a critical hit
    int damage = attackPower; // Base attack damage
    if (isCriticalHit()) {
        damage = getCriticalDamage(); // Adjust damage for critical hits
    }
    lastDamage = damage; // Store the damage dealt for tracking purposes
    target.takeDamage(damage); // Inflict damage to the target character
    std::cout << "Sun Wukong attacks and deals " << damage
                 << (isCriticalHit() ? " (Critical Damage!)" : "") << " to " << target.getName() << std::endl;
    target.takeDamage(damage); 
}

// Sun Wukong to defend and regain health
void SunWukong::defend() {
    std::cout << "Sun Wukong defends!\n";
    increaseHealth(5); // Heal by 5 health points
}

// Sun Wukong's special attack
void SunWukong::special(Character& target) {
    // Check cooldown status
    if (getCooldownTimer() > 0) {
        std::cout << "Sun Wukong tried to use special move, but it's on cooldown for "
                  << getCooldownTimer() << " more turns!\n";
        return;
    }
    // Check if enough special points are available
    if (specialPoints < 20) {
        std::cout << "Sun Wukong tried to use special move, but doesn't have enough special points! ("
                  << specialPoints << "/20)\n";
        return;
    }
    // Perform special attack
    extraDamage = std::max(12, attackPower + randomVariation(10)); // ensure minimum damage is 10
    std::cout << "Sun Wukong uses special move!" << extraDamage << "\n";
    target.takeDamage(extraDamage); // Apply the special attack damage
    specialPoints -= 20; // Deduct special points
    resetCooldown(5); // Reset cooldown to 5 turns
}

// render Sun Wukong's sprite on the screen
void SunWukong::draw(sf::RenderWindow& window, sf::Vector2f position) {
    sf::Texture texture;  // Create a texture object
    // Load the texture from the specified file path
    std::string path = std::filesystem::current_path().parent_path().string() + "/assets/sunwukong.png";
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load Sun Wukong texture from " << path << "!\n";
        return;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);
    
    window.draw(sprite); // Draw the sprite to the window
}

