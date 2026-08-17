#pragma once

#include <string>
#include <vector>
#include "Activity.h"
#include "Event.h"
#include "Map.h"
#include "Player.h"
#include "TimeSystem.h"

class Game {
public:
    Game();
    void run();

private:
    Player player;
    TimeSystem timeSystem;
    Map map;
    std::vector<Activity> activities;
    std::vector<Event> events;
    bool lastActionWasSleep;
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
    void enterMapMode();
    bool performActivity();
    void maybeTriggerEvent();
    void processExam();
    void trySaveGame() const;
    void displayFinalResult() const;
    int chooseDailyActivity() const;
};