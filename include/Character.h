#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class Character {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int specialPoints;
    
    bool criticalHit = false; // Whether this turn is a critical hit
    int lastDamage;   // Last damage received
    int criticalDamage; // Damage caused by critical hit
    int criticalChance = 60; // Critical hit chance as a percentage
    int blockedDamage; // Damage blocked by defense
    int specialRecovered; // Special points recovered
    int lastRecoveredHealth; // Record last recovered health points
    
    int cooldownTimer = 0;

    int randomVariation(int baseValue);

public:
    Character(const std::string& name, int health, int attackPower, int defense);
    const std::string& getName() const;

    virtual ~Character() = default;

    virtual void attack(Character& target);
    virtual void defend();
    virtual void special(Character& target);
    virtual void draw(sf::RenderWindow& window, sf::Vector2f position) = 0;

    void takeDamage(int damage);
    void increaseHealth(int amount);
    bool isAlive() const;
    
    void resetCriticalHit();
    void applyCriticalHit();

    int getHealth() const;
    int getSpecialPoints() const;
    int getLastDamage() const; // Get last damage taken
    int getCriticalDamage() const; // Get last critical damage caused
    int getBlockedDamage() const; // Get last damage blocked
    int getSpecialRecovered() const; // Get last special points recovered
    bool isCriticalHit() const; // Check if last turn was critical
    int getLastRecoveredHealth() const; // Method to retrieve the last recovered health
    
    // Cooldown functions
    void decreaseCooldown();
    bool canUseSpecial() const;
    void resetCooldown(int cooldown);
    int getCooldownTimer() const;
};

