#pragma once

#include <limits>
#include <string>
#include <iostream>

class InputManager {
public:
    template <typename T>
    static T promptInt(const std::string& prompt, T minValue, T maxValue) {
        while (true) {
            std::cout << prompt;
            T value;
            if (std::cin >> value && value >= minValue && value <= maxValue) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number between " << minValue << " and " << maxValue << ".\n";
        }
    }
};