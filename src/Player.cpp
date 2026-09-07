#include "Player.h"
#include <algorithm>
#include <sstream>

Player::Player()
    : health(80), energy(80), stress(20), happiness(60), knowledge(20), money(300), gpa(0.0), day(1), dayOfWeek(0), currentTimeMinutes(8 * 60), x(STARTING_X), y(STARTING_Y) {}

void Player::reset() {
    health = 80;
    energy = 80;
    stress = 20;
    happiness = 60;
    knowledge = 20;
    money = 300;
    gpa = 0.0;
    day = 1;
    dayOfWeek = 0;
    currentTimeMinutes = 8 * 60;
    x = STARTING_X;
    y = STARTING_Y;
}

void Player::clampStats() {
    health = std::clamp(health, 0, 100);
    energy = std::clamp(energy, 0, 100);
    stress = std::clamp(stress, 0, 100);
    happiness = std::clamp(happiness, 0, 100);
    knowledge = std::clamp(knowledge, 0, 100);
    money = std::max(money, 0);
    if (gpa < 0.0) {
        gpa = 0.0;
    } else if (gpa > 4.0) {
        gpa = 4.0;
    }
}

bool Player::isAlive() const {
    return health > 0 && happiness > 0 && stress < 100;
}

bool Player::hasFailed() const {
    return gpa < 1.5 || stress >= 100 || health <= 0 || happiness <= 0;
}

void Player::applyHealth(int amount) {
    health += amount;
}

void Player::applyEnergy(int amount) {
    energy += amount;
}

void Player::applyStress(int amount) {
    stress += amount;
}

void Player::applyHappiness(int amount) {
    happiness += amount;
}

void Player::applyKnowledge(int amount) {
    knowledge += amount;
}

void Player::applyMoney(int amount) {
    money += amount;
}

void Player::applyGpa(double amount) {
    gpa += amount;
}

void Player::advanceDay() {
    ++day;
    dayOfWeek = (dayOfWeek + 1) % 7;
}

int Player::getHealth() const {
    return health;
}

int Player::getEnergy() const {
    return energy;
}

int Player::getStress() const {
    return stress;
}

int Player::getHappiness() const {
    return happiness;
}

int Player::getKnowledge() const {
    return knowledge;
}

int Player::getMoney() const {
    return money;
}

double Player::getGpa() const {
    return gpa;
}

int Player::getDay() const {
    return day;
}

int Player::getDayOfWeek() const {
    return dayOfWeek;
}

int Player::getCurrentTime() const {
    return currentTimeMinutes;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

void Player::setDay(int value) {
    day = value;
}

void Player::setDayOfWeek(int value) {
    dayOfWeek = ((value % 7) + 7) % 7;
}

void Player::setCurrentTime(int value) {
    currentTimeMinutes = value;
}

void Player::setX(int value) {
    x = value;
}

void Player::setY(int value) {
    y = value;
}

void Player::setPosition(int xValue, int yValue) {
    x = xValue;
    y = yValue;
}

std::string Player::statusSummary() const {
    std::ostringstream output;
    output << "Day: " << day << "\n"
           << "\xE2\x9D\xA4\xEF\xB8\x8F Health: " << health << "%\n"
           << "\xE2\x9A\xA1 Energy: " << energy << "%\n"
           << "\xF0\x9F\x98\xB0 Stress: " << stress << "%\n"
           << "\xF0\x9F\x98\x8A Happiness: " << happiness << "%\n"
           << "\xF0\x9F\x93\x9A Knowledge: " << knowledge << "%\n"
           << "\xF0\x9F\x92\xB0 Money: $" << money << "\n"
           << "\xF0\x9F\x8E\x93 GPA: " << gpa << "\n";
    return output.str();
}
