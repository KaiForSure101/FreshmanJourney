#pragma once

#include <functional>
#include <string>

class Player;

struct Event {
    std::string name;
    std::string description;
    std::function<void(Player&)> effect;
};