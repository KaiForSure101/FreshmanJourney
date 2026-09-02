#pragma once

#include <string>
#include <vector>
#include "Event.h"
#include "Player.h"

// ============================================================
// Game.h
// The Game class owns the player, the event list, and the main
// game loop. It controls the 3-block-per-day progression and
// enforces all win/loss conditions.
//
// Flow:
//   run() -> startNewGame() or loadGame()
//         -> gameLoop()
//              - each day: maybeTriggerMorningEvent()
//              - each block: display status -> player picks action
//                            -> performAction()
//              - after 3 blocks: applyOvernightEffects()
//                                -> advance day
//         -> displayFinalResult()
// ============================================================

class Game {
public:
    Game();
    void run();

private:
    // The player object holds all 7 stats and the current day/block
    Player player;

    // The list of possible random morning events
    std::vector<Event> events;

    // How many days the semester lasts (win condition at day 30)
    const int semesterLength;

    // Path to the save file written to disk
    const std::string saveFilePath;

    // --- Setup ---
    void initEvents();      // Populate the events list at startup
    void showIntro() const; // Print the welcome message

    // --- Game flow ---
    void startNewGame(); // Reset player stats and begin the game loop
    void loadGame();     // Load from save file and continue
    void gameLoop();     // Main loop: 3 blocks per day for 30 days

    // --- Per-block helpers ---
    void displayDailyStatus() const;      // Print current stats header
    void performAction(int actionChoice); // Apply stat effects for chosen action

    // --- Per-day helpers ---
    // 30% chance each morning to trigger a random event
    // (e.g., Illness, Scholarship, Lost Wallet, etc.)
    void maybeTriggerMorningEvent();

    // Run at end of each day (after Block 3):
    //   - Stress > 80 damages health by 10
    //   - Energy restores +30 from overnight sleep
    //   - Stress decreases by 5 naturally overnight
    void applyOvernightEffects();

    // --- End of game ---
    // Shows final GPA, money, knowledge, and an ending message
    void displayFinalResult() const;

    // --- Save / Load ---
    void trySaveGame() const; // Write save file; prints success or failure
};