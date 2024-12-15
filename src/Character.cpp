#include "Character.h"
#include <algorithm>
#include <iostream>

// Constructor to initialize character attributes
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
      extraDamage(0),
      criticalChance(40), // Percentage chance for a critical hit
      blockedDamage(0),
      specialRecovered(0),
      lastRecoveredHealth(0),
      cooldownTimer(0) {
}

// Get the character's name
const std::string& Character::getName() const {
    return name; // Return the character's name
}


// Handle attack logic on a target character
void Character::attack(Character& target) {
    applyCriticalHit();
    int damage = attackPower;
    if (criticalHit) {
        damage = getCriticalDamage(); // Adjust damage for critical hits
    }
    lastDamage = damage; // Store the damage dealt for tracking
    std::cout << getName() << " deals " << damage << (isCriticalHit() ? " (Critical Damage!)" : "") << " to " << target.getName() << std::endl;
    target.takeDamage(damage); // Inflict the damage to the target
}

// Determine if a critical hit occurs
void Character::applyCriticalHit() {
    int randomValue = rand() % 100; // Generate a random value between 0 and 99
    criticalHit = ((randomValue < criticalChance) && !(randomValue & 1)); // Check critical chance and even value
        if (criticalHit) {
            criticalDamage = attackPower * 2; // Double damage for critical hits
        } else {
            criticalDamage = attackPower;
        }
    }

// Check if the last attack was a critical hit
bool Character::isCriticalHit() const {
    return criticalHit;
}

// Handle damage and adjust health and special points
void Character::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defense); // Reduce damage by defense, but ensure at least 1 damage
    blockedDamage = damage - actualDamage; // Record how much damage was blocked
    health = std::max(0, health - actualDamage); // Reduce health but ensure it doesn't go below 0
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
    health = std::min(maxHealth, health + amount); // Adjust health without exceeding maxHealth
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

// Perform a special attack on a target character
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

    // Execute special attack logic
    extraDamage = std::max(10, attackPower + randomVariation(10)); // Ensure minimum damage is 10
    target.takeDamage(extraDamage); // Apply the special attack damage
    specialPoints -= 20; // Deduct special points
    resetCooldown(5); // Reset cooldown to 5 turns
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

// Get the damage dealt in the last attack
int Character::getLastDamage() const {
    return lastDamage;
}

// Get the amount of damage blocked in the last attack
int Character::getBlockedDamage() const {
    return blockedDamage;
}

// Get the critical damage value
int Character::getCriticalDamage() const {
    return criticalDamage;
}

// Get the extra damage in a special attack
int Character::getExtraDamage() const {
    return extraDamage;
}

// Get the special points recovered after taking damage
int Character::getSpecialRecovered() const {
    return specialRecovered;
}

// Decrease the cooldown timer by 1 turn
void Character::decreaseCooldown() {
    if (cooldownTimer > 0) {
        cooldownTimer--;
    }
}

// Check if the special attack can be used
bool Character::canUseSpecial() const {
    return cooldownTimer == 0 && specialPoints >= 20;
}

// Reset the cooldown timer to a specified value
void Character::resetCooldown(int cooldown) {
    cooldownTimer = cooldown;
}

// Get the current value of the cooldown timer
int Character::getCooldownTimer() const {
    return cooldownTimer;
}

// Reset the critical hit status for the character
void Character::resetCriticalHit() {
    criticalHit = false;
}

