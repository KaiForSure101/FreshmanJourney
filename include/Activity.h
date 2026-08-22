#pragma once

#include <string>

struct Activity {
    std::string name;
    std::string description;
    int duration; // duration in minutes
    int healthChange;
    int energyChange;
    int stressChange;
    int happinessChange;
    int knowledgeChange;
    int moneyChange;
    double gpaChange;
};