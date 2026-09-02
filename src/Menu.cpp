#include "Menu.h"
#include "InputManager.h"
#include <iostream>

// ============================================================
// Menu.cpp
// Implements text-based menus for the campus life simulator.
// All menus use numbered options and InputManager to validate input.
// ============================================================

// ----------------------------
// promptMainMenu
// Shown once at program startup. Returns 1, 2, or 3.
// ----------------------------
int Menu::promptMainMenu() {
    std::cout << "\n";
    std::cout << "==========================================\n";
    std::cout << "         FRESHMAN JOURNEY                 \n";
    std::cout << "==========================================\n";
    std::cout << "  1. New Game\n";
    std::cout << "  2. Load Game\n";
    std::cout << "  3. Exit\n";
    std::cout << "==========================================\n";
    return InputManager::promptInt("Choose an option: ", 1, 3);
}

// ----------------------------
// promptActionMenu
// This is the MAIN gameplay menu, shown at the start of every
// time block. It tells the player what day and block they are on,
// then lists the 5 core actions plus Save & Exit.
//
// Every action costs exactly 1 time block.
// Returns the player's choice (1-6).
// ----------------------------
int Menu::promptActionMenu(int currentDay, int currentTimeBlock) {
    // Convert the numeric time block to a readable label
    std::string blockName;
    if (currentTimeBlock == 1) {
        blockName = "Morning";
    } else if (currentTimeBlock == 2) {
        blockName = "Afternoon";
    } else {
        blockName = "Evening";
    }

    std::cout << "\n";
    std::cout << "==========================================\n";
    std::cout << "  Day " << currentDay
              << " | " << blockName
              << " (Block " << currentTimeBlock << "/3)\n";
    std::cout << "==========================================\n";
    std::cout << "  What will you do this block?\n\n";

    // The 5 core actions — each costs 1 time block
    std::cout << "  1. Attend Class\n";
    std::cout << "     [+GPA, +Stress, -Energy]\n\n";

    std::cout << "  2. Study\n";
    std::cout << "     [+Intelligence, +Stress, -Energy, -Happiness]\n\n";

    std::cout << "  3. Relax / Hobby\n";
    std::cout << "     [+Happiness, -Stress, -Energy or -Money]\n\n";

    std::cout << "  4. Sleep / Rest\n";
    std::cout << "     [+Energy, +Health, -Stress]\n\n";

    std::cout << "  5. Work Part-Time\n";
    std::cout << "     [+Money, +Stress, -Energy]\n\n";

    std::cout << "  6. Save and Exit\n";
    std::cout << "==========================================\n";

    return InputManager::promptInt("Enter your choice (1-6): ", 1, 6);
}

// ----------------------------
// promptYesNo
// A reusable yes/no prompt. Keeps asking until valid input is given.
// Returns true for 'y'/'Y', false for 'n'/'N'.
// ----------------------------
bool Menu::promptYesNo(const std::string& message) {
    while (true) {
        std::cout << message << " (y/n): ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "y" || input == "Y") {
            return true;
        }
        if (input == "n" || input == "N") {
            return false;
        }

        std::cout << "Please type y or n.\n";
    }
}
