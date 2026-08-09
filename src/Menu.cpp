#include "Menu.h"
#include "InputManager.h"
#include <iostream>

int Menu::promptMainMenu() {
    std::cout << "=== Freshman Journey ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    return InputManager::promptInt("Choose an option: ", 1, 3);
}

int Menu::promptActivityMenu(const std::vector<Activity>& activities) {
    std::cout << "\nChoose your activity for today:\n";
    for (size_t index = 0; index < activities.size(); ++index) {
        const Activity& activity = activities[index];
        std::cout << index + 1 << ". " << activity.name << " - " << activity.description << "\n";
    }
    std::cout << activities.size() + 1 << ". Save and Exit\n";
    return InputManager::promptInt("Enter your choice: ", 1, static_cast<int>(activities.size() + 1));
}

bool Menu::promptYesNo(const std::string& message) {
    while (true) {
        std::cout << message << " (y/n): ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "y" || input == "Y") {
            return true;
        }
        if (input == "n" || input == "N") {
            return false;
        }
        std::cout << "Please answer y or n.\n";
    }
}
