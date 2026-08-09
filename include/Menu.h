#pragma once

#include <string>
#include <vector>
#include "Activity.h"

class Menu {
public:
    static int promptMainMenu();
    static int promptActivityMenu(const std::vector<Activity>& activities);
    static bool promptYesNo(const std::string& message);
};