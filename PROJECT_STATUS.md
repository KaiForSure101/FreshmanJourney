# Freshman Journey Project Status

Date: 2026-08-17

## Overview
Freshman Journey is a C++ console-based campus life simulation game. The project is organized as a small CMake-based application that lets the player manage a semester by balancing academics, wellbeing, money, and time.

## Current project structure
- `src/` — game implementation files
  - `main.cpp` — application entry point
  - `Game.cpp` — core game loop, activities, events, exams, and saving flow
  - `Player.cpp` — player state and stat logic
  - `Menu.cpp` — menu and choice handling
  - `SaveManager.cpp` — save/load behavior
  - `Utility.cpp` — helper/random utilities
- `include/` — header declarations for the game systems
- `data/` — runtime save data directory
- `build/` — generated CMake build output and executable
- `README.md` — basic project instructions
- `CMakeLists.txt` — build configuration for the project

## Current status
- The project is active and builds successfully.
- CMake configuration and compilation are working in the current workspace.
- The executable is available in `build/FreshmanJourney.exe` after a successful build.

## Functional status
The current game includes:
- semester progression and daily game loop
- multiple student activities with resource trade-offs
- random campus events
- exam scheduling and GPA effects
- save and load support for progress
- player health, energy, stress, happiness, knowledge, money, and GPA tracking

## Current gameplay
The game begins at the main menu with three choices: start a new semester, load a saved semester, or exit. A new game starts with a fresh player state and a 30-day semester. The player's initial values are:
- Health: 80
- Energy: 80
- Stress: 20
- Happiness: 60
- Knowledge: 20
- Money: $300
- GPA: 0.0
- Day: 1

Each day, the user selects one activity from the daily menu:
- Attend class
- Study
- Sleep
- Eat well
- Exercise
- Work part-time
- Join clubs
- Relax with friends
- Save and Exit

These activities affect the player's core stats. For example, attending class and studying raise knowledge and GPA, while sleep, eating well, and relaxing improve health or reduce stress. Working part-time adds money but consumes energy and may increase stress. The player must balance academics with personal wellbeing, because low health, low happiness, or excessive stress can cause the run to fail early.

After the activity is resolved, there is a chance for a random campus event (roughly one in three days). Events can boost mood or money, trigger stress, or harm the player's health. Every 7 days, the game triggers an exam that adjusts GPA based on the player's knowledge, energy, stress, and random variation. At the end of the day, the player may choose to save progress before moving to the next day.

The semester ends after 30 days or earlier if the player dies or becomes unable to continue. The final ending depends on the player's overall GPA, money, stress, and happiness. Ending states include Honor Student, Academic Failure, Financial Crisis, Burned Out, and Balanced Student.

## Build validation
The following commands were executed successfully in the project root:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

Result: build completed successfully with exit code 0.

## Notes
- There is no separate automated test suite present at the moment; validation is currently based on successful project compilation.
- The game expects a save path under `data/savegame.txt` for persistence.
- The project appears to be in a playable, development-ready state for a small console game prototype.
