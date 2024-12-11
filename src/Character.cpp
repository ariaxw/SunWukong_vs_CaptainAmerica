#include "Character.h"
#include <algorithm>
#include <iostream>

Character::Character(const std::string& name, int health, int attackPower, int defense)
    : name(name),
      health(health),
      maxHealth(health),
      attackPower(attackPower),
      defense(defense),
      specialPoints(0),
      criticalHit(false),
      lastDamage(0),
      criticalDamage(50),
      criticalChance(60),
      blockedDamage(0),
      specialRecovered(0),
      lastRecoveredHealth(0),
      cooldownTimer(0) {
}

const std::string& Character::getName() const {
    return name; // Return the character's name
}


// Handle attack logic
void Character::attack(Character& target) {
    applyCriticalHit();
    int damage = attackPower;
    if (criticalHit) {
        damage = getCriticalDamage();
    }
    lastDamage = damage;
    std::cout << getName() << " deals " << damage << (isCriticalHit() ? " (Critical Damage!)" : "") << " to " << target.getName() << std::endl;
    target.takeDamage(damage);
}

void Character::applyCriticalHit() {
    int randomValue = rand() % 100;
    criticalHit = ((randomValue < criticalChance) && !(randomValue & 1));
        if (criticalHit) {
            criticalDamage = attackPower * 2;
        } else {
            criticalDamage = attackPower;
        }
    }

bool Character::isCriticalHit() const {
    return criticalHit;
}

// Handle damage and adjust health and special points
void Character::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defense);
    blockedDamage = damage - actualDamage; // Record blocked damage
    health = std::max(0, health - actualDamage);
    specialRecovered = std::min(10, maxHealth - health); // Simple recovery logic
    // Increase special points upon taking damage
    specialPoints = std::min(100, specialPoints + specialRecovered);
}

// Add randomness to attacks
int Character::randomVariation(int baseValue) {
    return baseValue + (rand() % 5 - 2);  // Adds a random variation of -2 to 2
}

// Increase health by a specified amount, ensuring it does not exceed maxHealth
void Character::increaseHealth(int amount) {
    int oldHealth = health;
    health = std::min(maxHealth, health + amount);
    lastRecoveredHealth = health - oldHealth; // Calculate the actual recovered health
}

// Get the last recovered health points
int Character::getLastRecoveredHealth() const {
    return lastRecoveredHealth;
}

// Default defend implementation
void Character::defend() {
    // Default logic: reduce damage taken by a fixed percentage (e.g., 50%)
    blockedDamage = defense * 2; // Assume base defense blocks additional damage
    increaseHealth(blockedDamage / 2); // Default defend also regenerates some health

    std::cout << name << " defends and blocks " << blockedDamage
              << " damage while recovering " << (blockedDamage / 2) << " health!\n";
}

// Default special implementation
void Character::special(Character& target) {
    std::string errorMsg;
    if (getCooldownTimer() > 0) {
        errorMsg = name + " tried to use a special attack, but it's on cooldown for " +
                   std::to_string(getCooldownTimer()) + " more turns!";
    } else if (specialPoints < 20) {
        errorMsg = name + " tried to use a special attack, but doesn't have enough special points! (" +
                   std::to_string(specialPoints) + "/20)";
    }

    if (!errorMsg.empty()) {
        std::cout << errorMsg << std::endl;
        return;
    }

    // Perform special attack
    int extraDamage = std::max(10, attackPower + randomVariation(5)); // ensure minimum damage is 10
    lastDamage = extraDamage;
    target.takeDamage(extraDamage);

    specialPoints -= 20;
    resetCooldown(5);
    std::cout << name << " uses a special attack and causes " << extraDamage << " damage!\n";
}


// Check if the character is alive
bool Character::isAlive() const {
    return health > 0;
}

// Get the current health
int Character::getHealth() const {
    return health;
}

// Get the current special points
int Character::getSpecialPoints() const {
    return specialPoints;
}

int Character::getLastDamage() const {
    return lastDamage;
}

int Character::getBlockedDamage() const {
    return blockedDamage;
}

int Character::getCriticalDamage() const {
    return criticalDamage;
}


int Character::getSpecialRecovered() const {
    return specialRecovered;
}

void Character::decreaseCooldown() {
    if (cooldownTimer > 0) {
        cooldownTimer--;
    }
}

bool Character::canUseSpecial() const {
    return cooldownTimer == 0 && specialPoints >= 20;
}

void Character::resetCooldown(int cooldown) {
    cooldownTimer = cooldown;
}

int Character::getCooldownTimer() const {
    return cooldownTimer;
}

void Character::resetCriticalHit() {
    criticalHit = false;
}

