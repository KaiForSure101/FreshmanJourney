#pragma once

#include <string>
#include <vector>
#include "Player.h"
#include "TimeSystem.h"

enum class LocationType {
    CampusPath,
    Dorm,
    Library,
    Gym,
    StudentCenter,
    Classroom,
    Cafeteria
};

class Map {
public:
    explicit Map(Player& playerRef);

    void resetPlayerPosition();
    void setPlayerPosition(int x, int y);
    bool movePlayer(char direction, TimeSystem& timeSystem);
    void render(const TimeSystem& timeSystem) const;

    int getPlayerX() const;
    int getPlayerY() const;
    bool isWalkable(int x, int y) const;
    bool isInsideMap(int x, int y) const;
    LocationType getLocationType() const;
    std::string getLocationName() const;

private:
    std::vector<std::string> layout;
    Player* player;

    char getTileAt(int x, int y) const;
    static std::string locationToString(LocationType location);
};
