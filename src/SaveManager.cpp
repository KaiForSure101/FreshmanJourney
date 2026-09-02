#include "SaveManager.h"
#include <fstream>

// ============================================================
// SaveManager.cpp
// Reads and writes player stats to a plain text file.
//
// Save format — 9 values, one per line, in this order:
//   1. health
//   2. energy
//   3. stress
//   4. happiness
//   5. knowledge
//   6. money
//   7. gpa
//   8. day
//   9. timeBlock  (1=Morning, 2=Afternoon, 3=Evening)
// ============================================================

// ----------------------------
// save: write all player stats to a file.
// Returns true if the file was written successfully.
// ----------------------------
bool SaveManager::save(const Player& player, const std::string& filePath) {
    std::ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        return false; // Could not open or create the file
    }

    // Write each stat on its own line so the file is human-readable
    outputFile << player.getHealth()    << '\n';
    outputFile << player.getEnergy()    << '\n';
    outputFile << player.getStress()    << '\n';
    outputFile << player.getHappiness() << '\n';
    outputFile << player.getKnowledge() << '\n';
    outputFile << player.getMoney()     << '\n';
    outputFile << player.getGpa()       << '\n';
    outputFile << player.getDay()       << '\n';
    outputFile << player.getTimeBlock() << '\n';

    return outputFile.good();
}

// ----------------------------
// load: read all player stats from a file and restore them.
// Returns true if all 9 values were read without error.
// ----------------------------
bool SaveManager::load(Player& player, const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        return false; // File does not exist or cannot be opened
    }

    // Read each stat in the same order they were saved
    int    health;
    int    energy;
    int    stress;
    int    happiness;
    int    knowledge;
    int    money;
    double gpa;
    int    day;
    int    timeBlock;

    // If any value fails to parse, the save file is corrupt — abort load
    if (!(inputFile >> health >> energy >> stress >> happiness
                    >> knowledge >> money >> gpa >> day >> timeBlock)) {
        return false;
    }

    // Reset the player to default values first, then apply the loaded stats.
    // We do this by computing the difference from the default starting values
    // so we can use the existing applyXxx() methods cleanly.
    player.reset();
    player.applyHealth(health       - player.getHealth());
    player.applyEnergy(energy       - player.getEnergy());
    player.applyStress(stress       - player.getStress());
    player.applyHappiness(happiness - player.getHappiness());
    player.applyKnowledge(knowledge - player.getKnowledge());
    player.applyMoney(money         - player.getMoney());
    player.applyGpa(gpa             - player.getGpa());
    player.setDay(day);
    player.setTimeBlock(timeBlock);

    player.clampStats();
    return true;
}
