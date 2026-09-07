#pragma once

#include <random>
#include <string>

namespace Util {
    int clamp(int value, int minValue, int maxValue);
    int randomInt(int minValue, int maxValue);
    std::string secondsToTime(int totalSeconds);
}