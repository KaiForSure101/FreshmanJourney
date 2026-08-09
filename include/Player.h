#pragma once

#include <string>

class Player {
public:
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

    void setDay(int value);

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
};