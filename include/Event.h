#pragma once

#include <string>

// ============================================================
// Event.h
// Defines a random event that can happen at the start of a day.
//
// Each event is a plain data struct — just a name, a message to
// print, and integer stat changes. No function pointers or
// lambdas are used. Game.cpp applies the changes directly.
//
// Positive values = gain. Negative values = loss.
// A value of 0 means that stat is unaffected by this event.
// ============================================================

struct Event {
    std::string name;          // Short label, e.g. "Illness"
    std::string description;   // Message printed to the player

    // Stat changes caused by this event
    int    healthChange;
    int    energyChange;
    int    stressChange;
    int    happinessChange;
    int    knowledgeChange;
    int    moneyChange;
    double gpaChange;
};