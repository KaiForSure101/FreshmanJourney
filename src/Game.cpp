#include <iostream>
#include "Game.h"

using namespace std;

void Game::run()
{
    showMainMenu();
}

void Game::showMainMenu()
{
    int choice;

    cout << "==============================\n";
    cout << "     FRESHMAN JOURNEY\n";
    cout << "==============================\n";
    cout << "1. New Game\n";
    cout << "2. Load Game\n";
    cout << "3. Exit\n";

    cout << "\nChoose: ";
    cin >> choice;

    switch(choice)
    {
        case 1:
            cout << "\nStarting new game...\n";
            break;

        case 2:
            cout << "\nLoading save...\n";
            break;

        case 3:
            cout << "\nGoodbye!\n";
            break;

        default:
            cout << "\nInvalid choice.\n";
    }
}