#include "SaveManager.h"
#include "Player.h"
#include <fstream>
#include <sstream>

bool SaveManager::save(const Player& player, const std::string& filePath) {
    std::ofstream output(filePath);
    if (!output.is_open()) {
        return false;
    }

    output << player.getHealth() << '\n';
    output << player.getEnergy() << '\n';
    output << player.getStress() << '\n';
    output << player.getHappiness() << '\n';
    output << player.getKnowledge() << '\n';
    output << player.getMoney() << '\n';
    output << player.getGpa() << '\n';
    output << player.getDay() << '\n';
    output << player.getCurrentTime() << '\n';
    output << player.getX() << '\n';
    output << player.getY() << '\n';
    return output.good();
}

bool SaveManager::load(Player& player, const std::string& filePath) {
    std::ifstream input(filePath);
    if (!input.is_open()) {
        return false;
    }

    int health;
    int energy;
    int stress;
    int happiness;
    int knowledge;
    int money;
    double gpa;
    int day;
    int currentTime = 8 * 60;
    int x = Player::STARTING_X;
    int y = Player::STARTING_Y;

    if (!(input >> health >> energy >> stress >> happiness >> knowledge >> money >> gpa >> day)) {
        return false;
    }

    if (!(input >> currentTime)) {
        return false;
    }

    input >> x >> y;
    if (input.fail()) {
        input.clear();
        x = Player::STARTING_X;
        y = Player::STARTING_Y;
    }

    player.reset();
    player.applyHealth(health - player.getHealth());
    player.applyEnergy(energy - player.getEnergy());
    player.applyStress(stress - player.getStress());
    player.applyHappiness(happiness - player.getHappiness());
    player.applyKnowledge(knowledge - player.getKnowledge());
    player.applyMoney(money - player.getMoney());
    player.applyGpa(gpa - player.getGpa());
    player.setDay(day);
    player.setCurrentTime(currentTime);
    player.setPosition(x, y);
    player.clampStats();

    return true;
}
