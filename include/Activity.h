#pragma once

#include <string>

struct Activity {
    std::string name;
    std::string description;
    int energyCost;
    int healthChange;
    int stressChange;
    int happinessChange;
    int knowledgeChange;
    int moneyChange;
    double gpaChange;
};