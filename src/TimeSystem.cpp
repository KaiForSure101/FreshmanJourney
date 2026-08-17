#include "TimeSystem.h"
#include "Player.h"

namespace {
constexpr int MINUTES_PER_DAY = 24 * 60;
constexpr int MORNING_START = 8 * 60;
}

TimeSystem::TimeSystem(Player& playerRef)
    : player(&playerRef) {
    if (player != nullptr && player->getCurrentTime() <= 0) {
        player->setCurrentTime(MORNING_START);
    }
}

TimeSystem::TimeSystem(Player& playerRef, int initialMinutes)
    : player(&playerRef) {
    setCurrentTime(initialMinutes);
}

int TimeSystem::getCurrentTime() const {
    if (player == nullptr) {
        return 0;
    }
    return player->getCurrentTime();
}

void TimeSystem::setCurrentTime(int minutes) {
    if (player == nullptr) {
        return;
    }
    player->setCurrentTime(normalizeMinutes(minutes));
}

void TimeSystem::advanceTime(int minutes) {
    if (player == nullptr || minutes == 0) {
        return;
    }

    int currentMinutes = player->getCurrentTime();
    int updatedMinutes = currentMinutes + minutes;

    while (updatedMinutes >= MINUTES_PER_DAY) {
        player->setDay(player->getDay() + 1);
        updatedMinutes -= MINUTES_PER_DAY;
    }

    while (updatedMinutes < 0) {
        player->setDay(player->getDay() - 1);
        updatedMinutes += MINUTES_PER_DAY;
    }

    player->setCurrentTime(updatedMinutes);
}

void TimeSystem::sleep() {
    if (player == nullptr) {
        return;
    }

    player->setDay(player->getDay() + 1);
    player->setCurrentTime(MORNING_START);
}

std::string TimeSystem::formatCurrentTime() const {
    return formatMinutes(getCurrentTime());
}

std::string TimeSystem::formatMinutes(int totalMinutes) {
    int normalizedMinutes = normalizeMinutes(totalMinutes);
    int hours = normalizedMinutes / 60;
    int minutes = normalizedMinutes % 60;
    bool isPm = hours >= 12;

    int displayHour = hours % 12;
    if (displayHour == 0) {
        displayHour = 12;
    }

    std::string result;
    if (displayHour < 10) {
        result += "0";
    }
    result += std::to_string(displayHour) + ":";
    if (minutes < 10) {
        result += "0";
    }
    result += std::to_string(minutes) + " " + (isPm ? "PM" : "AM");
    return result;
}

int TimeSystem::normalizeMinutes(int totalMinutes) {
    int normalized = totalMinutes % MINUTES_PER_DAY;
    if (normalized < 0) {
        normalized += MINUTES_PER_DAY;
    }
    return normalized;
}
