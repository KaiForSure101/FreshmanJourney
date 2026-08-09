#pragma once

#include <string>
#include <vector>
#include "Activity.h"
#include "Event.h"
#include "Player.h"

class Game {
public:
    Game();
    void run();

private:
    Player player;
    std::vector<Activity> activities;
    std::vector<Event> events;
    const int semesterLength;
    const int examInterval;
    const std::string saveFilePath;

    void initActivities();
    void initEvents();
    void showIntro() const;
    void startNewGame();
    void loadGame();
    void gameLoop();
    void displayDailyReport() const;
    bool performActivity();
    void maybeTriggerEvent();
    void processExam();
    void trySaveGame() const;
    void displayFinalResult() const;
    int chooseDailyActivity() const;
};