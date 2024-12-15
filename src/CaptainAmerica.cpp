#include "CaptainAmerica.h"
#include <iostream>
#include <filesystem>

// perform an attack on a target character
void CaptainAmerica::attack(Character& target) {
    applyCriticalHit(); // Calculate if the attack is a critical hit
    int damage = attackPower; // Base attack damage
    if (isCriticalHit()) {
        damage = getCriticalDamage(); // Adjust damage for critical hits
    }
    lastDamage = damage;
    target.takeDamage(damage); // Apply damage to the target
    std::cout << "Captain America attacks and deals " << damage
                 << (isCriticalHit() ? " (Critical Damage!)" : "") << " to " << target.getName() << std::endl;
    target.takeDamage(damage); 
}

// defend and increase health
void CaptainAmerica::defend() {
    std::cout << "Captain America defends and increases the health!\n";
    increaseHealth(10); // Add 10 to the current health
}

// perform a special attack (Shield Throw) on a target character
void CaptainAmerica::special(Character& target) {
    // Check if the special ability is on cooldown
    if (getCooldownTimer() > 0) {
        std::cout << "Captain America tried to use shield throw, but it's on cooldown for "
                  << getCooldownTimer() << " more turns!\n";
        return;
    }
    // Check if there are enough special points to perform the move
    if (specialPoints < 20) {
        std::cout << "Captain America tried to use shield throw, but doesn't have enough special points! ("
                  << specialPoints << "/20)\n";
        return;
    }
    // Perform special attack
    extraDamage = std::max(10, attackPower + randomVariation(10)); // ensure minimum damage is 10
    std::cout << "Captain America uses shield throw!" << extraDamage << "\n";
    target.takeDamage(extraDamage); // Apply the special attack damage
    specialPoints -= 20; // Deduct the required special points
    resetCooldown(5); // Reset cooldown to 5 turns
}

// draw Captain America on the screen at a specified position
void CaptainAmerica::draw(sf::RenderWindow& window, sf::Vector2f position) {
    sf::Texture texture;
    // Get the file path for the texture
    std::string path = std::filesystem::current_path().parent_path().string() + "/assets/captainamerica.png";
    // Load the texture and handle errors if the file is missing
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load Captain America texture from " << path << "!\n";
        return;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f); // Scale down the sprite for better visibility
    
    // Draw elements in order
    window.draw(sprite);
}

