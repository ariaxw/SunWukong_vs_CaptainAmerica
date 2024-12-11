#include "SunWukong.h"
#include <iostream>
#include <filesystem>

void SunWukong::attack(Character& target) {
    applyCriticalHit();
    int damage = attackPower;
    if (isCriticalHit()) {
        damage = getCriticalDamage();
    }
    lastDamage = damage;
    target.takeDamage(damage);
    std::cout << "Sun Wukong attacks and deals " << damage
                 << (isCriticalHit() ? " (Critical Damage!)" : "") << " to " << target.getName() << std::endl;
    target.takeDamage(damage); 
}


void SunWukong::defend() {
    std::cout << "Sun Wukong defends!\n";
    increaseHealth(5);
}

void SunWukong::special(Character& target) {
    if (getCooldownTimer() > 0) {
        std::cout << "Sun Wukong tried to use special move, but it's on cooldown for "
                  << getCooldownTimer() << " more turns!\n";
        return;
    }
    if (specialPoints < 20) {
        std::cout << "Sun Wukong tried to use special move, but doesn't have enough special points! ("
                  << specialPoints << "/20)\n";
        return;
    }
    // Perform special attack
    extraDamage = std::max(12, attackPower + randomVariation(10)); // ensure minimum damage is 10
    std::cout << "Sun Wukong uses special move!" << extraDamage << "\n";
    target.takeDamage(extraDamage);
    specialPoints -= 20;
    resetCooldown(5); // Reset cooldown to 5 turns
}

void SunWukong::draw(sf::RenderWindow& window, sf::Vector2f position) {
    sf::Texture texture;
    std::string path = std::filesystem::current_path().parent_path().string() + "/assets/sunwukong.png";
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load Sun Wukong texture from " << path << "!\n";
        return;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);
    
    window.draw(sprite);
}

