#include "Game.h"
#include "Menu.h"
#include "SaveManager.h"
#include "InputManager.h"
#include "Utility.h"
#include <cctype>
#include <filesystem>
#include <iostream>
#include <limits>

Game::Game()
    : semesterLength(30), examInterval(7), saveFilePath("data/savegame.txt"), timeSystem(player), map(player),
      lastActionWasSleep(false), currentDayOfWeek(0), lastWarnedClassDay(-1), lastWarnedCourseIndex(-1) {
    initActivities();
    initEvents();
}

void Game::run() {
    showIntro();
    while (true) {
        int choice = Menu::promptMainMenu();
        switch (choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                loadGame();
                break;
            case 3:
                std::cout << "Good luck on campus!\n";
                return;
        }
    }
}

void Game::showIntro() const {
    std::cout << "\nWelcome to Freshman Journey!\n";
    std::cout << "You are beginning your first semester at university.\n";
    std::cout << "Make daily choices, manage your health, energy, stress,\n";
    std::cout << "happiness, knowledge, money, and GPA as you complete the semester.\n\n";
}

void Game::startNewGame() {
    player.reset();
    currentDayOfWeek = player.getDayOfWeek();
    map.resetPlayerPosition();
    std::filesystem::create_directories("data");
    std::cout << "Starting a new semester...\n\n";
    gameLoop();
}

void Game::loadGame() {
    if (!SaveManager::load(player, saveFilePath)) {
        std::cout << "No saved game found or save file is invalid.\n";
        return;
    }
    map.setPlayerPosition(player.getX(), player.getY());
    currentDayOfWeek = player.getDayOfWeek();
    std::cout << "Game loaded successfully.\n\n";
    gameLoop();
}

void Game::gameLoop() {
    bool active = true;
    while (active && player.getDay() <= semesterLength && player.isAlive()) {
        displayDailyReport();
        checkClassSchedule();
        int dayBefore = player.getDay();
        active = performActivity();
        if (!active) {
            break;
        }

        if (timeSystem.getCurrentTime() >= 22 * 60) {
            std::cout << "\xE2\x9A\xA0\xEF\xB8\x8F It's getting late. Time to sleep!\n";
            timeSystem.sleep();
        }

        int dayAfter = player.getDay();
        if (dayAfter > dayBefore) {
            handleDayTransition(dayBefore);

            if (!player.isAlive()) {
                break;
            }

            if (player.getDay() < semesterLength) {
                if (Menu::promptYesNo("Save progress before moving to the next day?")) {
                    trySaveGame();
                }
            }

            std::cout << "\n";
            continue;
        }

        // Same day: allow more actions. Events may still trigger between actions.
        maybeTriggerEvent();
        player.clampStats();

        if (!player.isAlive()) {
            break;
        }
    }

    displayFinalResult();
}

void Game::displayDailyReport() const {
    static const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    std::cout << "\n=== Day " << player.getDay() << " (" << dayNames[currentDayOfWeek] << ") ===\n";
    std::cout << "\xE2\x8F\xB0 Time: " << timeSystem.formatCurrentTime() << "\n";
    std::cout << player.statusSummary();
}

void Game::checkClassSchedule() {
    if (!schedule.isClassActive(timeSystem.getCurrentTime(), currentDayOfWeek)) {
        return;
    }

    std::vector<Course> courses = schedule.getCoursesForDay(currentDayOfWeek);
    for (int index = 0; index < static_cast<int>(courses.size()); ++index) {
        const Course& course = courses[index];
        if (timeSystem.getCurrentTime() < course.startTime ||
            timeSystem.getCurrentTime() >= course.endTime) {
            continue;
        }

        if (map.getLocationType() == LocationType::Classroom) {
            if (!schedule.isCourseAttended(currentDayOfWeek, index)) {
                player.applyKnowledge(course.knowledgeGain);
                schedule.markAttendance(currentDayOfWeek, index);
                std::cout << "\xE2\x9C\x85 Attended " << course.name << " class. Knowledge +" 
                          << course.knowledgeGain << ".\n";
            }
        } else if (lastWarnedClassDay != player.getDay() ||
                   lastWarnedCourseIndex != index) {
            std::cout << "\xE2\x9A\xA0\xEF\xB8\x8F Missing class: " << course.name
                      << " is in session now.\n";
            lastWarnedClassDay = player.getDay();
            lastWarnedCourseIndex = index;
        }
        break;
    }
}

void Game::handleDayTransition(int dayBefore) {
    (void)dayBefore;
    currentDayOfWeek = player.getDayOfWeek();
    maybeTriggerEvent();

    if (player.getDay() % examInterval == 0) {
        processExam();
        schedule.resetAttendance();
    }

    player.clampStats();
}

void Game::initActivities() {
    // At the top-level menu we only offer Sleep as a direct action.
    // Most activities are location-based and available when the player
    // is at a specific location on the map.
    activities = {
        {"Sleep", "Sleep to start the next day using the TimeSystem.", 0, 15, 15, -15, 10, 0, 0, 0.0}
    };
}

void Game::initEvents() {
    events = {
        {"Surprise Quiz", "A professor gives an unplanned quiz.", [](Player& player) {
            int bonus = player.getKnowledge() / 10;
            player.applyGpa(0.05 + bonus * 0.01);
            player.applyStress(10);
            player.applyHappiness(-5);
            std::cout << "\xF0\x9F\x8E\x89 Event: Surprise Quiz! Your knowledge helped you do well.\n";
        }},
        {"Club Invitation", "A student club invites you to a fun event.", [](Player& player) {
            player.applyHappiness(15);
            player.applyStress(-10);
            player.applyKnowledge(5);
            std::cout << "\xF0\x9F\x8E\x89 Event: Club Invitation! You made friends and enjoyed a break.\n";
        }},
        {"Rainy Day", "Bad weather slows you down and affects your mood.", [](Player& player) {
            player.applyEnergy(-10);
            player.applyHappiness(-10);
            player.applyStress(5);
            std::cout << "\xE2\x9A\xA0\xEF\xB8\x8F Event: Rainy Day. It feels tougher to stay motivated today.\n";
        }},
        {"Scholarship Offer", "A scholarship increases your financial stability.", [](Player& player) {
            player.applyMoney(80);
            player.applyHappiness(10);
            std::cout << "\xF0\x9F\x8E\x89 Event: Scholarship Offer! Your money improved.\n";
        }},
        {"Lost Wallet", "You misplace your wallet and lose some money.", [](Player& player) {
            player.applyMoney(-30);
            player.applyStress(10);
            std::cout << "\xE2\x9A\xA0\xEF\xB8\x8F Event: Lost Wallet. You need to be more careful with your budget.\n";
        }},
        {"Group Project", "A tight deadline raises stress but rewards academic progress.", [](Player& player) {
            player.applyKnowledge(8);
            player.applyStress(15);
            player.applyHappiness(-5);
            player.applyGpa(0.05);
            std::cout << "\xF0\x9F\x93\x9A Event: Group Project deadline. It was hard work, but you learned a lot.\n";
        }},
        {"Illness", "You catch a cold and need to rest.", [](Player& player) {
            player.applyHealth(-20);
            player.applyEnergy(-20);
            player.applyStress(10);
            player.applyHappiness(-10);
            std::cout << "\xE2\x9A\xA0\xEF\xB8\x8F Event: Illness. Take care of yourself and recover soon.\n";
        }},
        {"Campus Festival", "A festival boosts your happiness and relieves stress.", [](Player& player) {
            player.applyHappiness(20);
            player.applyStress(-15);
            player.applyEnergy(-10);
            player.applyMoney(-20);
            std::cout << "\xF0\x9F\x8E\x89 Event: Campus Festival! You had a fun and relaxing time.\n";
        }}
    };
}

int Game::chooseDailyActivity() const {
    return Menu::promptActivityMenu(activities);
}

void Game::enterMapMode() {
    while (true) {
        map.render(timeSystem);

        char input = ' ';
        std::cout << "\nChoose a direction: ";
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::toupper(static_cast<unsigned char>(input)) == 'Q') {
            return;
        }

        if (map.movePlayer(input, timeSystem)) {
            std::cout << "\nYou moved to " << map.getLocationName() << ".\n";
            if (map.getLocationType() != LocationType::CampusPath) {
                handleLocation();
            }
            if (timeSystem.getCurrentTime() >= 22 * 60) {
                return;
            }
        }
    }
}

bool Game::performActivity() {
    int selection = chooseDailyActivity();
    if (selection == static_cast<int>(activities.size()) + 2) {
        trySaveGame();
        return false;
    }

    if (selection == static_cast<int>(activities.size()) + 1) {
        enterMapMode();
        return true;
    }

    const Activity& activity = activities[selection - 1];
    std::cout << "\nYou chose: " << activity.name << "\n";

    if (activity.name == "Sleep") {
        timeSystem.sleep();
        std::cout << "You slept through the night and woke up at 08:00 AM.\n";
    } else {
        if (activity.energyChange < 0 && player.getEnergy() < -activity.energyChange) {
            std::cout << "You're too tired to do this.\n";
            return true;
        }

        player.applyHealth(activity.healthChange);
        player.applyEnergy(activity.energyChange);
        player.applyStress(activity.stressChange);
        player.applyHappiness(activity.happinessChange);
        player.applyKnowledge(activity.knowledgeChange);
        player.applyMoney(activity.moneyChange);
        player.applyGpa(activity.gpaChange);

        if (activity.duration > 0) {
            std::string before = timeSystem.formatCurrentTime();
            timeSystem.advanceTime(activity.duration);
            std::string after = timeSystem.formatCurrentTime();
            std::cout << "Time: " << before << " -> " << after << "\n";
            std::cout << "Time spent: " << activity.duration << " minutes\n";
        }
    }

    player.clampStats();
    std::cout << "\xE2\x9C\x85 Activity complete.\n";
    return true;
}

std::vector<Activity> Game::getActivitiesForLocation(LocationType location) const {
    std::vector<Activity> list;
    switch (location) {
        case LocationType::Dorm:
            list = {
                {"Rest", "Short rest in your dorm.", 60, 0, 20, -10, 0, 0, 0, 0.0},
                {"Study", "Study in your room.", 60, 0, -10, 5, 0, 5, 0, 0.0}
            };
            break;
        case LocationType::Library:
            list = {
                {"Study", "Focused study session.", 60, 0, -10, 5, 0, 8, 0, 0.0},
                {"Read", "Light reading.", 30, 0, -3, -3, 0, 3, 0, 0.0},
                {"Deep Study", "Long deep study.", 120, 0, -20, 10, 0, 15, 0, 0.0}
            };
            break;
        case LocationType::Gym:
            list = {
                {"Exercise", "Regular workout.", 60, 5, -15, -10, 0, 0, 0, 0.0},
                {"Heavy Training", "Intense training.", 120, 10, -25, -15, 0, 0, 0, 0.0},
                {"Rest", "Short rest at the gym.", 30, 0, 5, -5, 0, 0, 0, 0.0}
            };
            break;
        case LocationType::Cafeteria:
            list = {
                {"Eat", "Eat a full meal.", 30, 2, 15, 0, 0, 0, -10, 0.0},
                {"Cheap Meal", "A cheaper quick meal.", 20, 0, 8, 0, 0, 0, -5, 0.0},
                {"Social Meal", "Eat and socialize.", 60, 0, 5, 0, 8, 0, -15, 0.0}
            };
            break;
        case LocationType::StudentCenter:
            list = {
                {"Socialize", "Chat and meet people.", 60, 0, -5, -8, 10, 0, 0, 0.0},
                {"Relax", "Relax in the common area.", 30, 0, 0, -5, 3, 0, 0, 0.0},
                {"Club Activity", "Participate in a club.", 120, 0, -15, 0, 8, 3, 0, 0.0}
            };
            break;
        case LocationType::Classroom:
            list = {
                {"Review Notes", "Quick review of notes.", 30, 0, -5, 0, 0, 3, 0, 0.0},
                {"Study", "Study in the classroom.", 60, 0, -10, 5, 0, 6, 0, 0.0}
            };
            break;
        default:
            break;
    }
    return list;
}

void Game::handleLocation() {
    LocationType loc = map.getLocationType();
    std::string locName = map.getLocationName();
    auto locActivities = getActivitiesForLocation(loc);
    if (locActivities.empty()) {
        std::cout << "There is nothing to do here.\n";
        return;
    }

    std::cout << "Day: " << player.getDay() << "\n";
    std::cout << "Time: " << timeSystem.formatCurrentTime() << "\n";
    std::cout << "Energy: " << player.getEnergy() << "\n";

    int choice = Menu::promptLocationMenu(locActivities, locName);
    if (choice == static_cast<int>(locActivities.size()) + 1) {
        std::cout << "You leave the " << locName << ".\n";
        return;
    }

    const Activity& activity = locActivities[choice - 1];

    if (activity.energyChange < 0 && player.getEnergy() < -activity.energyChange) {
        std::cout << "You're too tired to do this.\n";
        return;
    }

    std::string before = timeSystem.formatCurrentTime();

    player.applyHealth(activity.healthChange);
    player.applyEnergy(activity.energyChange);
    player.applyStress(activity.stressChange);
    player.applyHappiness(activity.happinessChange);
    player.applyKnowledge(activity.knowledgeChange);
    player.applyMoney(activity.moneyChange);
    player.applyGpa(activity.gpaChange);

    if (activity.duration > 0) {
        timeSystem.advanceTime(activity.duration);
    }

    std::string after = timeSystem.formatCurrentTime();

    player.clampStats();

    std::cout << "\nActivity completed: " << activity.name << "\n";
    std::cout << "Time spent: " << activity.duration << " minutes\n";
    std::cout << "Time: " << before << " -> " << after << "\n";
    std::cout << player.statusSummary();
}

void Game::maybeTriggerEvent() {
    int chance = Util::randomInt(1, 100);
    if (chance <= 35) {
        int eventIndex = Util::randomInt(0, static_cast<int>(events.size() - 1));
        events[eventIndex].effect(player);
        player.clampStats();
    }
}

void Game::processExam() {
    std::cout << "\n\xF0\x9F\x93\x9D Exam day has arrived!\n";
    double knowledgeFactor = player.getKnowledge() * 0.02;
    double energyFactor = player.getEnergy() * 0.015;
    double stressFactor = player.getStress() * 0.01;
    double attendanceBonus = schedule.getAttendanceRate() * 0.1;
    double score = knowledgeFactor + energyFactor - stressFactor +
                   attendanceBonus + (Util::randomInt(0, 10) * 0.01);
    double gpaChange = 0.1 + score * 0.15;
    if (gpaChange < 0.0) {
        gpaChange = 0.0;
    }
    if (gpaChange > 0.4) {
        gpaChange = 0.4;
    }

    player.applyGpa(gpaChange);
    player.applyStress(10);
    player.applyHappiness(static_cast<int>(gpaChange * 10.0));
    player.applyEnergy(-10);
    player.clampStats();

    std::cout << "\xF0\x9F\x93\x8A Attendance: " << schedule.getAttendanceRate() * 100.0
              << "% (bonus +" << attendanceBonus << ")\n";
    std::cout << "\xE2\x9C\x85 Exam result: Your GPA improved by " << gpaChange << ".\n";
}

void Game::trySaveGame() const {
    if (SaveManager::save(player, saveFilePath)) {
        std::cout << "\xF0\x9F\x92\xBE Game saved to " << saveFilePath << ".\n";
    } else {
        std::cout << "Unable to save the game.\n";
    }
}

void Game::displayFinalResult() const {
    std::cout << "\n=== Semester Complete ===\n";
    std::cout << player.statusSummary();

    if (!player.isAlive()) {
        std::cout << "Your journey ended early due to burnout or poor health.\n";
        std::cout << "Ending: Burned Out\n";
        return;
    }

    if (player.getGpa() < 1.5) {
        std::cout << "Ending: Academic Failure\n";
    } else if (player.getMoney() < 50) {
        std::cout << "Ending: Financial Crisis\n";
    } else if (player.getGpa() >= 3.5 && player.getHappiness() >= 70) {
        std::cout << "Ending: Honor Student\n";
    } else if (player.getStress() >= 80) {
        std::cout << "Ending: Burned Out\n";
    } else {
        std::cout << "Ending: Balanced Student\n";
    }

    if (player.getGpa() >= 3.5 && player.getHappiness() >= 60) {
        std::cout << "Congratulations! You managed your academics and wellbeing well.\n";
    } else if (player.getGpa() >= 2.5) {
        std::cout << "You completed the semester with room to grow next time.\n";
    } else {
        std::cout << "Reflect on your choices and try to improve your balance in the next semester.\n";
    }
}
