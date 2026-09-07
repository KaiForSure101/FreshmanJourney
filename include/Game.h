#pragma once

#include <string>
#include <vector>
#include "Activity.h"
#include "Event.h"
#include "Map.h"
#include "Player.h"
#include "TimeSystem.h"
#include "Schedule.h"

class Game {
public:
    Game();
    void run();

private:
    Player player;
    TimeSystem timeSystem;
    Map map;
    Schedule schedule;
    std::vector<Activity> activities;
    std::vector<Event> events;
    bool lastActionWasSleep;
    const int semesterLength;
    const int examInterval;
    const std::string saveFilePath;
    int currentDayOfWeek;
    int lastWarnedClassDay;
    int lastWarnedCourseIndex;

    void initActivities();
    void initEvents();
    void showIntro() const;
    void startNewGame();
    void loadGame();
    void gameLoop();
    void displayDailyReport() const;
    void enterMapMode();
    bool performActivity();

    // Location-based activity helpers
    std::vector<Activity> getActivitiesForLocation(LocationType location) const;
    void handleLocation();

    void maybeTriggerEvent();
    void processExam();
    void trySaveGame() const;
    void displayFinalResult() const;
    int chooseDailyActivity() const;
    void checkClassSchedule();
    void handleDayTransition(int dayBefore);
};