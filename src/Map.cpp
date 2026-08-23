#include "Map.h"

#include <cctype>
#include <iostream>

Map::Map(Player& playerRef)
    : player(&playerRef) {
    layout = {
        "########################",
        "#D......#.......#.....L#",
        "#.......#.......#......#",
        "#.......#.......#......#",
        "#......................#",
        "###.###############.###",
        "#......................#",
        "#G........S.........C.#",
        "#......................#",
        "#..........F...........#",
        "########################"
    };

    resetPlayerPosition();
}

void Map::resetPlayerPosition() {
    if (player == nullptr) {
        return;
    }

    player->setPosition(Player::STARTING_X, Player::STARTING_Y);
}

void Map::setPlayerPosition(int x, int y) {
    if (player == nullptr) {
        return;
    }

    if (isInsideMap(x, y) && isWalkable(x, y)) {
        player->setPosition(x, y);
    }
    if (player == nullptr) {
        return false;
    }

    int newX = player->getX();
    int newY = player->getY();
    switch (std::toupper(static_cast<unsigned char>(direction))) {
        case 'W':
            --newY;
            break;
        case 'A':
            --newX;
            break;
        case 'S':
            ++newY;
            break;
        case 'D':
            ++newX;
            break;
        default:
            std::cout << "Use W, A, S, or D to move.\n";
            return false;
    }

    if (!isInsideMap(newX, newY) || !isWalkable(newX, newY)) {
        std::cout << "You cannot move there.\n";
        return false;
    }

    player->setPosition(newX, newY);
    timeSystem.advanceTime(3);
    return true;
}

void Map::render(const TimeSystem& timeSystem) const {
    std::cout << "==================================================\n";
    std::cout << "                    CAMPUS\n";
    std::cout << "==================================================\n\n";

    for (int y = 0; y < static_cast<int>(layout.size()); ++y) {
        for (int x = 0; x < static_cast<int>(layout[y].size()); ++x) {
            if (player != nullptr && player->getX() == x && player->getY() == y) {
                std::cout << 'u';
            } else {
                std::cout << layout[y][x];
            }
        }
        std::cout << '\n';
    }

    if (player != nullptr) {
        std::cout << "\nDay: " << player->getDay() << '\n';
        std::cout << "Time: " << timeSystem.formatCurrentTime() << "\n";
        std::cout << "Location: " << getLocationName() << "\n\n";
    }

    std::cout << "W/A/S/D: Move\n";
    std::cout << "Q: Exit Map\n";
}

int Map::getPlayerX() const {
    if (player == nullptr) {
        return 0;
    }
    return player->getX();
}

int Map::getPlayerY() const {
    if (player == nullptr) {
        return 0;
    }
    return player->getY();
}

bool Map::isWalkable(int x, int y) const {
    if (!isInsideMap(x, y)) {
        return false;
    }
    return getTileAt(x, y) != '#';
}

bool Map::isInsideMap(int x, int y) const {
    if (y < 0 || y >= static_cast<int>(layout.size())) {
        return false;
    }
    if (x < 0 || x >= static_cast<int>(layout[y].size())) {
        return false;
    }
    return true;
}

LocationType Map::getLocationType() const {
    if (player == nullptr) {
        return LocationType::CampusPath;
    }

    char tile = getTileAt(player->getX(), player->getY());
    switch (tile) {
        case 'D':
            return LocationType::Dorm;
        case 'L':
            return LocationType::Library;
        case 'G':
            return LocationType::Gym;
        case 'S':
            return LocationType::StudentCenter;
        case 'C':
            return LocationType::Classroom;
        case 'F':
            return LocationType::Cafeteria;
        default:
            return LocationType::CampusPath;
    }
}

std::string Map::getLocationName() const {
    return locationToString(getLocationType());
}

char Map::getTileAt(int x, int y) const {
    if (!isInsideMap(x, y)) {
        return '#';
    }
    return layout[y][x];
}

std::string Map::locationToString(LocationType location) {
    switch (location) {
        case LocationType::Dorm:
            return "Dorm";
        case LocationType::Library:
            return "Library";
        case LocationType::Gym:
            return "Gym";
        case LocationType::StudentCenter:
            return "Student Center";
        case LocationType::Classroom:
            return "Classroom";
        case LocationType::Cafeteria:
            return "Cafeteria";
        default:
            return "Campus Path";
    }
}
