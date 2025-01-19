# TaflSole

A command-line implementation of the ancient Nordic board game Tafl (also known as Hnefatafl) written in C++.

## Overview

TaflSole is a two-player, text-based version of the strategic board game Tafl. Players take alternating turns moving pieces on a board, with one player controlling the defenders (including a king piece) and the other controlling the attackers. The game features a comprehensive move system, board visualization, and game state management.

## Getting Started

### Program Options

When you start the program, you will be presented with two options:

1. `newgame` - Start a new game of Tafl
2. `quit` - Exit the program

### Gameplay

After selecting `newgame`, the game board will be displayed, and the attacker always makes the first move. The game continues until either the king escapes or is captured.

## Commands

The following commands are available during gameplay:

- `move <from_coordinates> <to_coordinates>` - Move a piece from one position to another
  - Example: `move a1 a3`
- `back` - Undo the previous move
- `info` - Display the current game status, including captured pieces for each player
- `help` - Show all available commands and their usage

## Project Structure

The project is organized into several modules, each handling specific aspects of the game:

### Source Files

- `TaflSole.cpp`
  - Contains the main entry point
  - Calls the `run()` function from the game module

### Header and Implementation Files

- `board.h/cpp`
  - Manages board-specific functionality
  - Handles board state and piece placement

- `history.h/cpp`
  - Implements move history tracking
  - Provides functionality for undoing moves

- `logic.h/cpp`
  - Contains core game logic
  - Manages rules and victory conditions

- `game.h/cpp`
  - Handles user interface and input processing
  - Manages game flow and main game loop

- `positionChecks.h/cpp`
  - Contains three commonly reused position checking functions
  - Centralizes specific validation methods used across other modules

- `constants.h`
  - Defines game constants
  - Contains frequently used values across modules

## Technical Details

- Language: C++
- Interface: Command-line
- Input: Text commands
- Output: ASCII board representation
- Data Structures: Uses structs without constructors for object representation

## Development Notes

This project was developed as a first-year, first-semester university project, demonstrating fundamental programming concepts including:

- File organization and modular design
- Command parsing and user input handling
- Game state management
- Turn-based gameplay implementation
- Data structure usage with structs

## Author

[Lubomir Topalski](https://github.com/Nsgshsk),
[Sofia University "St. Kliment Ohridski"](https://uni-sofia.bg/index.php/eng),
[Faculty of Mathematics and Informatics](https://fmi.uni-sofia.bg/en),
[Winter semester 2024-2025]()