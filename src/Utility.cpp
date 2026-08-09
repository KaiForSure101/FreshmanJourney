#include "Utility.h"
#include <algorithm>
#include <chrono>

int Util::clamp(int value, int minValue, int maxValue) {
    return std::clamp(value, minValue, maxValue);
}

int Util::randomInt(int minValue, int maxValue) {
    static std::mt19937 generator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(minValue, maxValue);
    return distribution(generator);
}

std::string Util::secondsToTime(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}
