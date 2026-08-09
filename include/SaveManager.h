#pragma once

#include <string>

class Player;

class SaveManager {
public:
    static bool save(const Player& player, const std::string& filePath);
    static bool load(Player& player, const std::string& filePath);
};