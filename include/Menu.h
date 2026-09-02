#pragma once

#include <string>

// ============================================================
// Menu.h
// Handles all text-based menus shown to the player.
//
// Design: all methods are static so Game.cpp can call them
// without needing a Menu object — keeps things simple.
// ============================================================

class Menu {
public:
    // Shown at startup: New Game / Load Game / Exit
    static int promptMainMenu();

    // Shown each time block: displays day, block name, and the
    // 5 core actions plus a Save & Exit option.
    // Returns the player's choice (1-6).
    static int promptActionMenu(int currentDay, int currentTimeBlock);

    // A simple yes/no prompt used when confirming save or quit.
    // Returns true for 'y', false for 'n'.
    static bool promptYesNo(const std::string& message);
};