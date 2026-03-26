#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

#include "Game.h"

using namespace std;

int main() {

    int choice;

    do {
        cout << "\n========== FIFTEEN GAME ==========\n1. Solo Mode (You play)\n2. AI Mode (Computer plays)\n0. Exit\nChoose:";

        cin >> choice;

        if (choice == 1 || choice == 2) {
            int sizeChoice;
            cout << "\nSelect board size:\n 1. 8-puzzle (3x3)\n 2. 15-puzzle (4x4)\n Choice: ";
            cin >> sizeChoice;
            int size = (sizeChoice == 1) ? 3 : 4;

  
            initBoard(size);

            int shuffleChoice;
            cout << "\nShuffle mode:\n 1. Random\n 2. Manual\n Choice: ";
            cin >> shuffleChoice;

            if (shuffleChoice == 1) {
                shuffle(100);
            }
            else {
                manualShuffle();
            }

            if (choice == 1) {
                cout << "\nSolo game begins!" << endl;
                while (!isWin()) {
                    drawBoard();
                    int moveVal;
                    cout << "Enter number to move (0 to quit): ";
                    cin >> moveVal;

                    if (moveVal == 0) break;

                    if (!makeMove(moveVal)) {
                        cout << "Invalid move! It must be near the empty space.\n";
                        system("pause");
                    }
                }

                if (isWin()) {
                    drawBoard();
                    cout << "\nCongrats! You solved it!\n";
                    saveStats();
                    system("pause");
                }
            }
            else if (choice == 2) {
                cout << "\nComputer mode begins..." << endl;
                solveAI();
                system("pause");
            }
        }

    } while (choice != 0);

    cout << "Goodbye!" << endl;
    return 0;
}