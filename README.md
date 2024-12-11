# SunWukong vs CaptainAmerica

### A competitive 2D battle game featuring Sun Wukong and Captain America with unique abilities and strategic gameplay!

---

## Overview

"SunWukong vs CaptainAmerica" is a turn-based 2D battle game where players can choose to play as Sun Wukong or Captain America. Each character has unique abilities, health points, and special points, offering strategic depth to the gameplay.


## Features

- **Two Iconic Characters**: Sun Wukong and Captain America, each with distinct abilities.
- **Dynamic Turn-Based Gameplay**: Engaging mechanics for attacking, defending, or special abilities.
- **Strategic Mechanics**: Manage health and special points effectively to outsmart your opponent.
- **Custom Graphics and Animations**: Includes custom sprites, shield, effects, and animations.
- **Cross-Platform**: Compatible with Windows, macOS, and Linux using CMake.


## Installation
### Prerequisites
1. A C++ compiler supporting C++17 or later.
2. CMake installed.
3. SDL2 development libraries for rendering.
### Steps
1. Clone the repository:
```
git clone <repository_url>
cd SunWukong_vs_CaptainAmerica
```
2. Create a build directory:
```
mkdir build
cd build
```
3. Build the project:
```
cmake ..
make
```
4. Run the game:
```
./SunWukong_vs_CaptainAmerica
```

## Usage

- **Start the Game**: Run the built executable.
- **Controls**:
  * A: Attack - Deal damage to the opponent.
  * D: Defend - Reduce incoming damage and recover some health.
  * S: Special Attack - Use special points to unleash a high-damage attack.

## Game Mechanics
### Core Mechanics
1. **Health Points (HP)**: Represent the life of each character. Drops to zero when the character loses.
2. **Attack**: Basic action dealing damage based on attack power, with a chance for critical hits.
3. **Defense**: Reduces incoming damage and regenerates health.
4. **Special Attack**: High-damage moves that cost SP and may have cooldowns.
5. **Special Points (SP)**: Required for special attacks. Gain SP by taking or blocking damage.

### Characters
#### Sun Wukong
- **Health**: Moderate
- **Attack Power**: High
- **Special Ability**: Adds bonus damage (+15) to attacks.
- **Unique Traits**:
  * Special attack costs 10 SP.
  * Can deal critical hits with high damage.
    
#### Captain America
- **Health**: High
- **Attack Power**: Moderate
- **Special Ability**: Uses a shield throw for extra damage (+10).
- **Unique Traits**:
  * Special attack costs 10 SP.
  * Focuses on defense and health regeneration.

### Gameplay
- Each turn, choose between attack, defense, or special attack.
- Monitor health and special points to balance offense and defense.
- Exploit critical hits and special moves to gain an advantage.
- There are three rounds in one game

## File Structure
```
SunWukong_vs_CaptainAmerica/
├── assets/                # Game assets like sprites and fonts
│   ├── sunwukong.png
│   ├── captainamerica.png
│   ├── shield.png
│   ├── arial.ttf
├── include/               # Header files
│   ├── Character.h
│   ├── Game.h
│   ├── CaptainAmerica.h
│   ├── SunWukong.h
├── src/                   # Source files
│   ├── main.cpp
│   ├── Character.cpp
│   ├── Game.cpp
│   ├── CaptainAmerica.cpp
│   ├── SunWukong.cpp
├── build/                 # Build directory
├── CMakeLists.txt         # Build configuration
```

## Dependencies
1. **SDL2 Library**: Used for rendering and input handling.
2. **CMake**: Build configuration.
3. **Standard C++ Libraries**: STL features for logic and data management.

## Contributing

Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a feature branch.
3. Commit your changes.
4. Submit a pull request.
