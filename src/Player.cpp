#include "Player.h"
#include <algorithm>   // for std::clamp
#include <sstream>     // for std::ostringstream
#include <iomanip>     // for std::fixed and std::setprecision

// ============================================================
// Player.cpp
// Implements all stat management methods for the Player class.
// See Player.h for the full list of stats and their rules.
// ============================================================

// ----------------------------
// Constructor: set starting values
// ----------------------------
Player::Player()
    : health(80),
      energy(80),
      stress(20),
      happiness(60),
      knowledge(20),
      money(300),
      gpa(0.0),
      day(1),
      currentTimeBlock(1) {
    // Starting values reflect a typical freshman: decent health/energy,
    // low stress, moderate happiness, limited knowledge and money.
}

// ----------------------------
// reset: called when starting a new game
// ----------------------------
void Player::reset() {
    health           = 80;
    energy           = 80;
    stress           = 20;
    happiness        = 60;
    knowledge        = 20;
    money            = 300;
    gpa              = 0.0;
    day              = 1;
    currentTimeBlock = 1;
}

// ----------------------------
// clampStats: keeps every stat within its valid range.
// Call this after any series of stat changes to prevent
// values from going out of bounds (e.g., health > 100).
// ----------------------------
void Player::clampStats() {
    health    = std::clamp(health,    0, 100);
    energy    = std::clamp(energy,    0, 100);
    stress    = std::clamp(stress,    0, 100);
    happiness = std::clamp(happiness, 0, 100);
    knowledge = std::clamp(knowledge, 0, 100);

    // Money cannot go below zero (no debt mechanic)
    if (money < 0) {
        money = 0;
    }

    // GPA is capped between 0.0 and 4.0
    if (gpa < 0.0) {
        gpa = 0.0;
    } else if (gpa > 4.0) {
        gpa = 4.0;
    }
}

// ----------------------------
// isAlive: returns false when health OR energy hits 0.
// Both conditions end the game immediately.
// ----------------------------
bool Player::isAlive() const {
    return health > 0 && energy > 0;
}

// ----------------------------
// Stat modifier methods
// These add (or subtract, if negative) from each stat.
// Always call clampStats() after applying a set of changes.
// ----------------------------
void Player::applyHealth(int amount)    { health    += amount; }
void Player::applyEnergy(int amount)    { energy    += amount; }
void Player::applyStress(int amount)    { stress    += amount; }
void Player::applyHappiness(int amount) { happiness += amount; }
void Player::applyKnowledge(int amount) { knowledge += amount; }
void Player::applyMoney(int amount)     { money     += amount; }
void Player::applyGpa(double amount)    { gpa       += amount; }

// ----------------------------
// advanceDay: called at the end of each full day (after Block 3)
// ----------------------------
void Player::advanceDay() {
    day++;
}

// ----------------------------
// Getters
// ----------------------------
int    Player::getHealth()    const { return health;           }
int    Player::getEnergy()    const { return energy;           }
int    Player::getStress()    const { return stress;           }
int    Player::getHappiness() const { return happiness;        }
int    Player::getKnowledge() const { return knowledge;        }
int    Player::getMoney()     const { return money;            }
double Player::getGpa()       const { return gpa;             }
int    Player::getDay()       const { return day;              }
int    Player::getTimeBlock() const { return currentTimeBlock; }

// ----------------------------
// Setters (used only by SaveManager when restoring a saved game)
// ----------------------------
void Player::setDay(int value)       { day              = value; }
void Player::setTimeBlock(int value) { currentTimeBlock = value; }

// ----------------------------
// statusSummary: builds a formatted string showing all stats.
// Called each turn so the player always sees their current state.
// ----------------------------
std::string Player::statusSummary() const {
    // Convert the numeric time block to a human-readable label
    std::string blockName;
    if (currentTimeBlock == 1) {
        blockName = "Morning";
    } else if (currentTimeBlock == 2) {
        blockName = "Afternoon";
    } else {
        blockName = "Evening";
    }

    std::ostringstream output;
    output << "Day: " << day << " | " << blockName << " (Block " << currentTimeBlock << "/3)\n";
    output << "------------------------------------------\n";
    output << "  Health    : " << health    << " / 100\n";
    output << "  Energy    : " << energy    << " / 100\n";
    output << "  Happiness : " << happiness << " / 100\n";
    output << "  Stress    : " << stress    << " / 100"
           << (stress > 80 ? "  *** HIGH! Damages Health overnight ***" : "") << "\n";
    output << "  Knowledge : " << knowledge << " / 100\n";
    output << "  GPA       : " << std::fixed << std::setprecision(2) << gpa << " / 4.00\n";
    output << "  Money     : $" << money << "\n";
    output << "------------------------------------------\n";
    return output.str();
}
