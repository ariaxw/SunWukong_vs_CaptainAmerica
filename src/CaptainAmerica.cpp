#include "CaptainAmerica.h"
#include <iostream>
#include <filesystem>

void CaptainAmerica::attack(Character& target) {
    applyCriticalHit();
    int damage = attackPower;
    if (isCriticalHit()) {
        damage = getCriticalDamage();
    }
    lastDamage = damage;
    target.takeDamage(damage);
    std::cout << "Captain America attacks and deals " << damage
                 << (isCriticalHit() ? " (Critical Damage!)" : "") << " to " << target.getName() << std::endl;
    target.takeDamage(damage); 
}

void CaptainAmerica::defend() {
    std::cout << "Captain America defends and increases the health!\n";
    increaseHealth(10);
}

void CaptainAmerica::special(Character& target) {
    if (getCooldownTimer() > 0) {
        std::cout << "Captain America tried to use shield throw, but it's on cooldown for "
                  << getCooldownTimer() << " more turns!\n";
        return;
    }
    if (specialPoints < 20) {
        std::cout << "Captain America tried to use shield throw, but doesn't have enough special points! ("
                  << specialPoints << "/20)\n";
        return;
    }
    // Perform special attack
    std::cout << "Captain America uses shield throw!\n";
    target.takeDamage(attackPower + 10);
    specialPoints -= 20;
    resetCooldown(5); // Reset cooldown to 5 turns
}

void CaptainAmerica::draw(sf::RenderWindow& window, sf::Vector2f position) {
    sf::Texture texture;
    std::string path = std::filesystem::current_path().parent_path().string() + "/assets/captainamerica.png";
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load Captain America texture from " << path << "!\n";
        return;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);
    
    // Draw elements in order
    window.draw(sprite);
}

