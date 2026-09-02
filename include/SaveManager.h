#pragma once

#include <string>
#include "Player.h"

// ============================================================
// SaveManager.h
// Handles reading and writing the player's stats to a text file.
//
// Save format (one value per line):
//   health, energy, stress, happiness, knowledge,
//   money, gpa, day, timeBlock
// ============================================================

class SaveManager {
public:
    // Write player stats to the given file path.
    // Returns true on success, false if the file could not be opened.
    static bool save(const Player& player, const std::string& filePath);

    // Read player stats from the given file path.
    // Returns true on success, false if the file is missing or corrupt.
    static bool load(Player& player, const std::string& filePath);
};