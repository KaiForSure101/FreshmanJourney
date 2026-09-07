#pragma once

#include <string>

class Player;

class TimeSystem {
public:
    explicit TimeSystem(Player& player);
    TimeSystem(Player& player, int initialMinutes);

    int getCurrentTime() const;
    void setCurrentTime(int minutes);
    void advanceTime(int minutes);
    void sleep();
    std::string formatCurrentTime() const;

    static std::string formatMinutes(int totalMinutes);
    static int normalizeMinutes(int totalMinutes);

private:
    Player* player;
};
