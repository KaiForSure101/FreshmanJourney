#include "Game.h"
#include "Menu.h"
#include "SaveManager.h"
#include "Utility.h"
#include "InputManager.h"
#include <filesystem>
#include <iostream>

// ============================================================
// Game.cpp
// Implements the main game loop and all gameplay logic.
//
// Key design decisions:
//   - Time is split into 3 blocks per day (Morning, Afternoon,
//     Evening). Each action costs exactly 1 block.
//   - After Evening, overnight effects run automatically.
//   - A 30% random event fires each Morning before the player acts.
//   - Stat changes from actions use simple addition/subtraction.
//   - GPA gained from Attend Class is scaled by Knowledge:
//       gpaGain = 0.02 + (knowledge / 100.0) * 0.03
//     So a student with Knowledge=100 gains twice as much GPA
//     per class as one with Knowledge=0.
// ============================================================

// ----------------------------
// Constructor
// ----------------------------
Game::Game()
    : semesterLength(30),
      saveFilePath("data/savegame.txt") {
    initEvents();
}

// ----------------------------
// run: entry point called from main.cpp.
// Loops on the main menu until the player exits.
// ----------------------------
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

// ----------------------------
// showIntro: prints once at startup
// ----------------------------
void Game::showIntro() const {
    std::cout << "\n";
    std::cout << "Welcome to Freshman Journey!\n";
    std::cout << "You are beginning your first semester at university.\n";
    std::cout << "Balance your Health, Energy, Stress, Happiness,\n";
    std::cout << "Knowledge, GPA, and Money over 30 days.\n";
    std::cout << "Each day has 3 time blocks. Every action costs 1 block.\n\n";
}

// ----------------------------
// startNewGame: resets all stats and begins the game loop
// ----------------------------
void Game::startNewGame() {
    player.reset();
    // Make sure the save folder exists so we can write later
    std::filesystem::create_directories("data");
    std::cout << "Starting a new semester...\n\n";
    gameLoop();
}

// ----------------------------
// loadGame: reads player stats from disk, then continues the loop
// ----------------------------
void Game::loadGame() {
    if (!SaveManager::load(player, saveFilePath)) {
        std::cout << "No saved game found or save file is invalid.\n";
        return;
    }
    std::cout << "Game loaded successfully.\n\n";
    gameLoop();
}

// ----------------------------
// gameLoop: the heart of the game.
//
// Each iteration of the outer while-loop is ONE TIME BLOCK.
// When 3 blocks have been used, the day ends:
//   1. applyOvernightEffects() runs (stress penalty, energy restore)
//   2. Player is prompted to save
//   3. Day counter advances and block resets to 1 (Morning)
//   4. A morning event may fire at the start of the new day
//
// The loop ends when:
//   - The player's health or energy hits 0 (Game Over)
//   - 30 days are completed (Win)
//   - The player saves and exits
// ----------------------------
void Game::gameLoop() {
    bool keepPlaying = true;

    while (keepPlaying && player.isAlive() && player.getDay() <= semesterLength) {

        // At the start of every Morning (block 1), check for a random event.
        // This gives the player a sense of unpredictability each new day.
        if (player.getTimeBlock() == 1) {
            maybeTriggerMorningEvent();
        }

        // Check again after the event, in case it killed the player (e.g., illness)
        if (!player.isAlive()) {
            break;
        }

        // Show current stats so the player knows their situation
        displayDailyStatus();

        // Show the action menu and get the player's choice
        int actionChoice = Menu::promptActionMenu(player.getDay(), player.getTimeBlock());

        // Option 6 = Save and Exit
        if (actionChoice == 6) {
            trySaveGame();
            keepPlaying = false;
            break;
        }

        // Apply the chosen action's stat effects
        performAction(actionChoice);

        // Clamp all stats into their valid ranges after changes
        player.clampStats();

        // Check if the player died from the action (e.g., energy hit 0)
        if (!player.isAlive()) {
            break;
        }

        // Advance the time block.
        // If we just finished Evening (block 3), end the day.
        int currentBlock = player.getTimeBlock();
        if (currentBlock == 3) {
            // The day is over — run overnight effects
            std::cout << "\n--- Night falls. Day " << player.getDay() << " is over. ---\n";
            applyOvernightEffects();
            player.clampStats();

            // Check if overnight effects killed the player (e.g., stress damage)
            if (!player.isAlive()) {
                break;
            }

            // Offer to save at the end of each day
            if (player.getDay() < semesterLength) {
                if (Menu::promptYesNo("Save progress before the next day?")) {
                    trySaveGame();
                }
            }

            // Move to the next day, starting at Morning (block 1)
            player.advanceDay();
            player.setTimeBlock(1);
            std::cout << "\n";

        } else {
            // Just move to the next block within the same day
            player.setTimeBlock(currentBlock + 1);
        }
    }

    // Show the end screen (win or game over)
    displayFinalResult();
}

// ----------------------------
// displayDailyStatus: prints the player's current stats.
// Called at the start of every time block so the player can
// make an informed decision about which action to choose.
// ----------------------------
void Game::displayDailyStatus() const {
    std::cout << "\n";
    std::cout << player.statusSummary();
}

// ----------------------------
// performAction: applies the stat changes for one chosen action.
//
// Each case is a separate action with clearly labeled effects.
// Comments explain WHY each stat changes, not just what changes.
// ----------------------------
void Game::performAction(int actionChoice) {
    switch (actionChoice) {

        case 1: { // Attend Class
            // Going to class increases GPA. The bonus is scaled by
            // Knowledge — a more prepared student benefits more from lectures.
            double gpaGain = 0.02 + (player.getKnowledge() / 100.0) * 0.03;

            player.applyGpa(gpaGain);
            player.applyStress(10);   // Deadlines and coursework add stress
            player.applyEnergy(-15);  // Sitting through lectures drains energy

            std::cout << "\nYou attended class.\n";
            std::cout << "  GPA +" << std::fixed; // gpa shown in displayDailyStatus
            std::cout << "  Stress +10, Energy -15\n";
            break;
        }

        case 2: { // Study
            // Studying builds Knowledge permanently and improves GPA slightly.
            // It costs energy and reduces happiness (it's not fun).
            player.applyKnowledge(5);
            player.applyGpa(0.01);
            player.applyStress(8);
            player.applyEnergy(-15);
            player.applyHappiness(-5);

            std::cout << "\nYou studied for a block.\n";
            std::cout << "  Knowledge +5, GPA +0.01\n";
            std::cout << "  Stress +8, Energy -15, Happiness -5\n";
            break;
        }

        case 3: { // Relax / Hobby
            // Relaxing restores happiness and cuts stress.
            // It costs money (you go out or buy something) and a little energy.
            player.applyHappiness(15);
            player.applyStress(-12);
            player.applyEnergy(-10);
            player.applyMoney(-15);

            std::cout << "\nYou relaxed and enjoyed a hobby.\n";
            std::cout << "  Happiness +15, Stress -12\n";
            std::cout << "  Energy -10, Money -$15\n";
            break;
        }

        case 4: { // Sleep / Rest
            // Resting during the day is less effective than overnight sleep,
            // but it's a useful emergency recovery if energy is critically low.
            player.applyEnergy(25);
            player.applyHealth(5);
            player.applyStress(-8);

            std::cout << "\nYou rested and recovered some energy.\n";
            std::cout << "  Energy +25, Health +5, Stress -8\n";
            break;
        }

        case 5: { // Work Part-Time
            // Working earns money but burns energy and adds stress.
            // It's necessary for financial stability but easy to overdo.
            player.applyMoney(40);
            player.applyEnergy(-20);
            player.applyStress(12);

            std::cout << "\nYou worked a part-time shift.\n";
            std::cout << "  Money +$40, Stress +12, Energy -20\n";
            break;
        }

        default:
            // This should never happen because InputManager validates the range.
            std::cout << "Invalid action. Please try again.\n";
            break;
    }
}

// ----------------------------
// maybeTriggerMorningEvent: 30% chance each day.
//
// Picks a random event from the list and applies all its stat
// changes directly using player.applyXxx() calls.
// No lambdas — each Event is a plain struct of integers.
// ----------------------------
void Game::maybeTriggerMorningEvent() {
    // Roll a number 1-100; trigger only if it lands in the bottom 30%
    int roll = Util::randomInt(1, 100);
    if (roll > 30) {
        return; // No event today — lucky!
    }

    // Pick a random event from the list
    int eventIndex = Util::randomInt(0, static_cast<int>(events.size()) - 1);
    const Event& randomEvent = events[eventIndex];

    // Print the event description so the player knows what happened
    std::cout << "\n*** Morning Event: " << randomEvent.name << " ***\n";
    std::cout << randomEvent.description << "\n";

    // Apply all the stat changes stored in the Event struct.
    // This is straightforward — no function calls, no lambdas.
    player.applyHealth(randomEvent.healthChange);
    player.applyEnergy(randomEvent.energyChange);
    player.applyStress(randomEvent.stressChange);
    player.applyHappiness(randomEvent.happinessChange);
    player.applyKnowledge(randomEvent.knowledgeChange);
    player.applyMoney(randomEvent.moneyChange);
    player.applyGpa(randomEvent.gpaChange);

    player.clampStats();
}

// ----------------------------
// applyOvernightEffects: runs automatically at the end of each day.
//
// High stress is dangerous — it physically wears the body down.
// Overnight sleep partially restores energy and eases stress.
// These effects simulate the natural rhythm of a student's week.
// ----------------------------
void Game::applyOvernightEffects() {
    // High stress (>80) damages health — the body cannot sustain that level
    if (player.getStress() > 80) {
        player.applyHealth(-10);
        std::cout << "Your high stress damaged your health overnight! (Health -10)\n";
    }

    // Sleeping overnight restores a significant portion of energy
    player.applyEnergy(30);
    std::cout << "You slept overnight. (Energy +30)\n";

    // Stress decreases slightly with rest — but not much without hobbies
    player.applyStress(-5);
    std::cout << "Your stress eased a little overnight. (Stress -5)\n";
}

// ----------------------------
// initEvents: creates the list of random morning events.
//
// Each event is a plain struct — name, description, and flat
// integer/double stat changes. No lambdas or function pointers.
// Game.cpp applies the changes directly in maybeTriggerMorningEvent().
// ----------------------------
void Game::initEvents() {
    events = {
        // name, description, health, energy, stress, happiness, knowledge, money, gpa
        {
            "Surprise Quiz",
            "A professor gives an unplanned quiz. Your preparation pays off!",
            0, 0, 10, -5, 0, 0, 0.05
        },
        {
            "Club Invitation",
            "A student club invites you to a fun event. You make new friends!",
            0, 0, -10, 15, 5, 0, 0.0
        },
        {
            "Rainy Day",
            "Bad weather slows you down and affects your mood.",
            0, -10, 5, -10, 0, 0, 0.0
        },
        {
            "Scholarship Offer",
            "You receive a small scholarship. Your financial stress eases!",
            0, 0, 0, 10, 0, 80, 0.0
        },
        {
            "Lost Wallet",
            "You misplaced your wallet and lost some cash. Stay organized!",
            0, 0, 10, -5, 0, -30, 0.0
        },
        {
            "Group Project",
            "A tight group project deadline arrives. Hard work, but worth it.",
            0, 0, 15, -5, 8, 0, 0.05
        },
        {
            "Illness",
            "You caught a cold. Rest up and take care of yourself.",
            -15, -20, 10, -10, 0, 0, 0.0
        },
        {
            "Campus Festival",
            "There is a campus festival today! You enjoy a fun, relaxing morning.",
            0, -10, -15, 20, 0, -20, 0.0
        },
        {
            "Found $20",
            "You found $20 on the ground near the cafeteria. Lucky day!",
            0, 0, 0, 5, 0, 20, 0.0
        }
    };
}

// ----------------------------
// trySaveGame: writes player stats to disk.
// Prints a success or failure message.
// ----------------------------
void Game::trySaveGame() const {
    if (SaveManager::save(player, saveFilePath)) {
        std::cout << "Game saved to " << saveFilePath << ".\n";
    } else {
        std::cout << "Could not save the game. Check that the data/ folder exists.\n";
    }
}

// ----------------------------
// displayFinalResult: shown at the end of the semester or on Game Over.
//
// Evaluates the player's performance across multiple metrics and
// prints an appropriate ending message.
// ----------------------------
void Game::displayFinalResult() const {
    std::cout << "\n==========================================\n";
    std::cout << "           SEMESTER COMPLETE              \n";
    std::cout << "==========================================\n";
    std::cout << player.statusSummary();

    // --- Game Over check ---
    if (!player.isAlive()) {
        std::cout << "\nYour journey ended early.\n";
        if (player.getHealth() <= 0) {
            std::cout << "Your health dropped to zero from overwork and stress.\n";
        } else {
            std::cout << "You ran out of energy and could not continue.\n";
        }
        std::cout << "Ending: Burned Out\n";
        std::cout << "Tip: Next time, use Relax and Rest actions more often.\n";
        return;
    }

    // --- Evaluate performance across 3 key metrics ---
    double finalGpa   = player.getGpa();
    int    finalMoney = player.getMoney();
    int    finalKnow  = player.getKnowledge();

    std::cout << "\n--- Final Evaluation ---\n";

    // Pick an ending label based on GPA, money, and happiness
    if (finalGpa >= 3.5 && player.getHappiness() >= 70) {
        std::cout << "Ending: Honor Student\n";
        std::cout << "You excelled academically and stayed happy. Outstanding!\n";
    } else if (finalGpa < 1.5) {
        std::cout << "Ending: Academic Struggle\n";
        std::cout << "Your GPA needs improvement. Attend more classes next time.\n";
    } else if (finalMoney < 50) {
        std::cout << "Ending: Financial Strain\n";
        std::cout << "You made it through, but barely have money left. Work more!\n";
    } else if (player.getStress() >= 80) {
        std::cout << "Ending: Burned Out (survived)\n";
        std::cout << "You survived but your stress is dangerously high. Rest more!\n";
    } else {
        std::cout << "Ending: Balanced Student\n";
        std::cout << "You completed the semester with solid balance. Well done!\n";
    }

    // Print a detailed summary line
    std::cout << "\nFinal GPA      : " << std::fixed << std::setprecision(2) << finalGpa << " / 4.00\n";
    std::cout << "Final Knowledge: " << finalKnow << " / 100\n";
    std::cout << "Final Money    : $" << finalMoney << "\n";
    std::cout << "==========================================\n";
}
