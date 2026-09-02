#pragma once

#include <string>

// ============================================================
// Player.h
// Holds all 7 core stats and the current day/time block.
//
// Stats and their ranges:
//   Health    : 0-100  (0 = Game Over)
//   Energy    : 0-100  (0 = Game Over)
//   Happiness : 0-100
//   Stress    : 0-100  (>80 damages health overnight)
//   Knowledge : 0-100  (scales GPA gains from class/study)
//   GPA       : 0.0-4.0
//   Money     : >= 0   (no upper cap)
//
// Time is tracked in discrete blocks per day:
//   Block 1 = Morning
//   Block 2 = Afternoon
//   Block 3 = Evening
// After Block 3, the day ends and overnight effects apply.
// ============================================================

class Player {
public:
    Player();

    // Reset all stats back to starting values (used for New Game)
    void reset();

    // Force all stats to stay within their legal bounds
    void clampStats();

    // Returns false if health or energy reaches 0 (triggers Game Over)
    bool isAlive() const;

    // --- Stat modifiers ---
    // Each "apply" method adds or subtracts the given amount from a stat.
    // clampStats() should be called after applying changes.
    void applyHealth(int amount);
    void applyEnergy(int amount);
    void applyStress(int amount);
    void applyHappiness(int amount);
    void applyKnowledge(int amount);
    void applyMoney(int amount);
    void applyGpa(double amount);

    // Increments the day counter by 1 (called at end of each day)
    void advanceDay();

    // --- Getters ---
    int    getHealth()    const;
    int    getEnergy()    const;
    int    getStress()    const;
    int    getHappiness() const;
    int    getKnowledge() const;
    int    getMoney()     const;
    double getGpa()       const;
    int    getDay()       const;
    int    getTimeBlock() const;  // returns 1, 2, or 3

    // --- Setters (used by SaveManager when loading a game) ---
    void setDay(int value);
    void setTimeBlock(int value);

    // Returns all stats as a formatted multi-line string for display
    std::string statusSummary() const;

private:
    int    health;
    int    energy;
    int    stress;
    int    happiness;
    int    knowledge;
    int    money;
    double gpa;
    int    day;
    int    currentTimeBlock; // 1=Morning, 2=Afternoon, 3=Evening
};