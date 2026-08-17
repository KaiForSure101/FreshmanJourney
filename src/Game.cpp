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
    : semesterLength(30), examInterval(7), saveFilePath("data/savegame.txt"), timeSystem(player), map(player), lastActionWasSleep(false) {
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
    std::cout << "Game loaded successfully.\n\n";
    gameLoop();
}

void Game::gameLoop() {
    bool active = true;
    while (active && player.getDay() <= semesterLength && player.isAlive()) {
        displayDailyReport();
        active = performActivity();
        if (!active) {
            break;
        }

        if (lastActionWasSleep) {
            lastActionWasSleep = false;
            continue;
        }

        maybeTriggerEvent();

        if (player.getDay() % examInterval == 0) {
            processExam();
        }

        player.clampStats();

        if (!player.isAlive()) {
            break;
        }

        if (player.getDay() < semesterLength) {
            if (Menu::promptYesNo("Save progress before moving to the next day?")) {
                trySaveGame();
            }
        }
        player.advanceDay();
        std::cout << "\n";
    }

    displayFinalResult();
}

void Game::displayDailyReport() const {
    std::cout << "\n=== Day " << player.getDay() << " ===\n";
    std::cout << "Time: " << timeSystem.formatCurrentTime() << "\n";
    std::cout << player.statusSummary();
}

void Game::initActivities() {
    activities = {
        {"Attend class", "Improve knowledge and GPA at the cost of energy.", 20, 0, 10, -5, 10, 0, 0.12},
        {"Study", "Study hard to boost knowledge and exam readiness.", 15, -5, 15, -10, 15, 0, 0.08},
        {"Sleep", "Recover energy and health while reducing stress.", 0, 15, -15, -20, 10, 0, 0.0},
        {"Eat well", "Restore health and happiness with a nutritious meal.", 0, 10, -5, 10, 0, -20, 0.0},
        {"Exercise", "Improve physical health and happiness, but lower energy.", 15, 10, 10, 10, 0, 0, 0.0},
        {"Work part-time", "Earn money, at the expense of energy and stress.", 20, -10, 15, -10, 0, 40, 0.0},
        {"Join clubs", "Build happiness and reduce stress while making friends.", 10, 5, -10, 20, 0, -10, 0.02},
        {"Relax with friends", "Recharge mental health and lower stress.", 5, 5, -15, 20, 0, -15, 0.0}
    };
}

void Game::initEvents() {
    events = {
        {"Surprise Quiz", "A professor gives an unplanned quiz.", [](Player& player) {
            int bonus = player.getKnowledge() / 10;
            player.applyGpa(0.05 + bonus * 0.01);
            player.applyStress(10);
            player.applyHappiness(-5);
            std::cout << "Event: Surprise Quiz! Your knowledge helped you do well.\n";
        }},
        {"Club Invitation", "A student club invites you to a fun event.", [](Player& player) {
            player.applyHappiness(15);
            player.applyStress(-10);
            player.applyKnowledge(5);
            std::cout << "Event: Club Invitation! You made friends and enjoyed a break.\n";
        }},
        {"Rainy Day", "Bad weather slows you down and affects your mood.", [](Player& player) {
            player.applyEnergy(-10);
            player.applyHappiness(-10);
            player.applyStress(5);
            std::cout << "Event: Rainy Day. It feels tougher to stay motivated today.\n";
        }},
        {"Scholarship Offer", "A scholarship increases your financial stability.", [](Player& player) {
            player.applyMoney(80);
            player.applyHappiness(10);
            std::cout << "Event: Scholarship Offer! Your money improved.\n";
        }},
        {"Lost Wallet", "You misplace your wallet and lose some money.", [](Player& player) {
            player.applyMoney(-30);
            player.applyStress(10);
            std::cout << "Event: Lost Wallet. You need to be more careful with your budget.\n";
        }},
        {"Group Project", "A tight deadline raises stress but rewards academic progress.", [](Player& player) {
            player.applyKnowledge(8);
            player.applyStress(15);
            player.applyHappiness(-5);
            player.applyGpa(0.05);
            std::cout << "Event: Group Project deadline. It was hard work, but you learned a lot.\n";
        }},
        {"Illness", "You catch a cold and need to rest.", [](Player& player) {
            player.applyHealth(-20);
            player.applyEnergy(-20);
            player.applyStress(10);
            player.applyHappiness(-10);
            std::cout << "Event: Illness. Take care of yourself and recover soon.\n";
        }},
        {"Campus Festival", "A festival boosts your happiness and relieves stress.", [](Player& player) {
            player.applyHappiness(20);
            player.applyStress(-15);
            player.applyEnergy(-10);
            player.applyMoney(-20);
            std::cout << "Event: Campus Festival! You had a fun and relaxing time.\n";
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
    lastActionWasSleep = false;

    if (activity.name == "Sleep") {
        timeSystem.sleep();
        lastActionWasSleep = true;
        std::cout << "You slept through the night and woke up at 08:00 AM.\n";
    } else {
        player.applyEnergy(-activity.energyCost);
        player.applyHealth(activity.healthChange);
        player.applyStress(activity.stressChange);
        player.applyHappiness(activity.happinessChange);
        player.applyKnowledge(activity.knowledgeChange);
        player.applyMoney(activity.moneyChange);
        player.applyGpa(activity.gpaChange);

        if (activity.energyCost > player.getEnergy()) {
            player.applyHealth(-5);
            player.applyStress(5);
            std::cout << "You are pushing yourself too hard without enough energy.\n";
        }
    }

    player.clampStats();
    std::cout << "Activity complete.\n";
    return true;
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
    std::cout << "\nExam day has arrived!\n";
    double knowledgeFactor = player.getKnowledge() * 0.02;
    double energyFactor = player.getEnergy() * 0.015;
    double stressFactor = player.getStress() * 0.01;
    double score = knowledgeFactor + energyFactor - stressFactor + (Util::randomInt(0, 10) * 0.01);
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

    std::cout << "Exam result: Your GPA improved by " << gpaChange << ".\n";
}

void Game::trySaveGame() const {
    if (SaveManager::save(player, saveFilePath)) {
        std::cout << "Game saved to " << saveFilePath << ".\n";
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
