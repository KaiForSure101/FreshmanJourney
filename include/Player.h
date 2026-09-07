#pragma once

#include <string>

class Player {
public:
    static const int STARTING_X = 1;
    static const int STARTING_Y = 1;

    Player();

    void reset();
    void clampStats();

    bool isAlive() const;
    bool hasFailed() const;

    void applyHealth(int amount);
    void applyEnergy(int amount);
    void applyStress(int amount);
    void applyHappiness(int amount);
    void applyKnowledge(int amount);
    void applyMoney(int amount);
    void applyGpa(double amount);
    void advanceDay();

    int getHealth() const;
    int getEnergy() const;
    int getStress() const;
    int getHappiness() const;
    int getKnowledge() const;
    int getMoney() const;
    double getGpa() const;
    int getDay() const;
    int getDayOfWeek() const;
    int getCurrentTime() const;
    int getX() const;
    int getY() const;

    void setDay(int value);
    void setDayOfWeek(int value);
    void setCurrentTime(int value);
    void setX(int value);
    void setY(int value);
    void setPosition(int x, int y);

    std::string statusSummary() const;

private:
    int health;
    int energy;
    int stress;
    int happiness;
    int knowledge;
    int money;
    double gpa;
    int day;
    int dayOfWeek;
    int currentTimeMinutes;
    int x;
    int y;
};