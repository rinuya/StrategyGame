# Simple Strategy Game

## Overview

This is a turn-based strategy game inspired by classics like Civilization, Total War: Warhammer, Risk, and Age of Empires. Players gather resources, expand territories, construct buildings, and engage in tactical combat in a pixel-art, top-down world. The ultimate goal: conquer your opponent’s Townhall.

With its **Client (UI)** and **Server (GameState)** architecture, the game is designed for local play against an AI or another player, with future extensibility for networked multiplayer.

---

## Core Gameplay Features

1. **Resource Management**  
   Gather resources like food, wood, gold, and stone by constructing specialized buildings. These resources are essential for territorial expansion, army deployment, and sustaining your empire.  

2. **Territorial Expansion**  
   Start with a single tile and grow by colonizing neutral tiles or conquering enemy ones. Each additional tile increases resource demands, encouraging thoughtful strategy.  

3. **Building Construction**  
   Place buildings on owned tiles to generate resources or other bonuses. Balancing expansion with resource upkeep is key to success.  

4. **Combat**  
   Deploy armies to protect your territories or invade opponents. Strategic positioning and resource planning are critical in battle.  

5. **Custom Maps**  
   Use the map editor to design your own battlegrounds, save them as text files, and challenge your friends.  

---

## Instructions for Building and Using the Software

### Prerequisites

#### Development Tools
- **C++ Compiler**: GCC (MinGW for Windows) or Clang  
- **Build System**: CMake 3.28 or newer
- **Graphics Library**: SFML 2.5 or newer  
- **Version Control**: Git (optional)  

#### Dependencies
- **SFML**: Used for graphics rendering. Install instructions below.  

### Setting Up the Environment

#### Install SFML
- **On macOS (using Homebrew)**:  
  ```bash
  brew install sfml
  ```
- **On Linux**:  
  ```bash
  sudo apt-get install libsfml-dev
  ```
- **On Windows**:  
  - Download SFML binaries from the [SFML website](https://www.sfml-dev.org/).  
  - Follow the instructions for integrating SFML with your compiler.  

#### Clone the Repository
Pull the latest code from the repository:  
```bash
git clone <repository_url>
cd <repo_name>
```

---

### How to Compile the Program

#### Using CMake
1. **Create a build directory**:  
   ```bash
   mkdir build
   cd build
   ```
2. **Generate build files**:  
   ```bash
   cmake ..
   ```
3. **Compile the code**:  
   ```bash
   make
   ```
4. **Run the game**:  
   ```bash
   ./bin/main
   ```

---

### Basic User Guide

1. **Starting the Game**  
   Run the executable to begin a game against a PlayerAI opponent or another player.  

2. **Gameplay**  
   - **Turn System**: Use the turn button (bottom-right) to end your turn when ready.  
   - **Townhall Actions**: Recruit armies and manage resources from your Townhall.  
   - **Army Movement**: Move armies to adjacent tiles and colonize neutral zones or engage in combat with enemies.  
   - **Building Construction**: Place resource-generating buildings on owned tiles.  

3. **UI Features**  
   - **Resource Display**: Track resources in the top-right corner.  
   - **Info Box**: Check details about the selected tile, building, or army on the left.  
   - **Map Editor**: Create custom maps via a simple, user-friendly interface.  

---

## Testing

Thorough testing is integrated into the build process to validate each main class. The tests ensure the correctness, stability, and functionality of the code.  

### Running Tests
1. **Build Tests**  
   Run the following command from the `build` directory:  
   ```bash
   make
   ```
2. **Execute Tests**  
   Use `ctest` to run all tests:  
   ```bash
   ctest
   ```
   The tests automatically validate core functionalities such as resource management, combat, and building mechanics. Individual test files are located in the `tests` directory.  
