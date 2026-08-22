#pragma once

#include <string>
#include <vector>
#include "Activity.h"

class Menu {
public:
    static int promptMainMenu();
    static int promptActivityMenu(const std::vector<Activity>& activities);
    static int promptLocationMenu(const std::vector<Activity>& activities, const std::string& locationName);
    static bool promptYesNo(const std::string& message);
};