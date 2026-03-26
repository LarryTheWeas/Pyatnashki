#include <windows.h> 
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <queue>
#include <map>

#include "Game.h"

using namespace std;

static vector<vector<int>> board;
static int boardSize;
static int emptyX, emptyY;
static Stats stats;

void initBoard(int s) {
    boardSize = s;
    board.assign(s, vector<int>(s));
    int count = 1;
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < s; j++) {
            if (count < s * s) {
                board[i][j] = count++;
            }
            else {
                board[i][j] = 0;
                emptyX = i;
                emptyY = j;
            }
        }
    }
    stats.moves = 0;
}

void drawBoard() {
    system("cls");
    cout << "Moves performed: " << stats.moves << "\n";
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == 0)
                cout << setw(4) << "";
            else
                cout << setw(4) << board[i][j];
        }
        cout << "\n\n";
    }
}

bool makeMove(int val) {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == val) {
                if (abs(i - emptyX) + abs(j - emptyY) == 1) {
                    swap(board[i][j], board[emptyX][emptyY]);
                    emptyX = i;
                    emptyY = j;
                    stats.moves++;
                    return true;
                }
            }
        }
    }
    return false;
}

void shuffle(int steps) {
    srand((unsigned int)time(0));
    int dX[] = { 0, 0, 1, -1 };
    int dY[] = { 1, -1, 0, 0 };
    int count = 0;
    while (count < steps) {
        int dir = rand() % 4;
        int nX = emptyX + dX[dir];
        int nY = emptyY + dY[dir];
        if (nX >= 0 && nX < boardSize && nY >= 0 && nY < boardSize) {
            swap(board[emptyX][emptyY], board[nX][nY]);
            emptyX = nX; emptyY = nY;
            count++;
        }
    }
    stats.moves = 0;
}

void manualShuffle() {
    system("cls");
    cout << "Enter " << boardSize * boardSize << " numbers (0 for empty):\n";
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            cin >> board[i][j];
            if (board[i][j] == 0) {
                emptyX = i; emptyY = j;
            }
        }
    }
    stats.moves = 0;
}

bool isWin() {
    int expected = 1;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (i == boardSize - 1 && j == boardSize - 1) return board[i][j] == 0;
            if (board[i][j] != expected++) return false;
        }
    }
    return true;
}

int calcManhattan(const vector<vector<int>>& b) {
    int dist = 0;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (b[i][j] != 0) {
                int targetX = (b[i][j] - 1) / boardSize;
                int targetY = (b[i][j] - 1) % boardSize;
                dist += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return dist;
}

void saveStats() {
    ofstream outFile("gameStats.txt", ios::app);
    if (outFile.is_open()) {
        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);

        outFile << "--- Game Session ---" << endl;
        outFile << "Date: " << dt;
        outFile << "Board Size: " << boardSize << "x" << boardSize << endl;
        outFile << "Total Moves: " << stats.moves << endl;
        outFile.close();
        cout << "Stats successfully saved\n";
    }
    else {
        cout << "Error! File couldn't be opened\n";
    }
}

void solveAI() {
    priority_queue<State, vector<State>, greater<State>> pq;
    map<vector<vector<int>>, bool> visited;

    pq.push(State(board, emptyX, emptyY, 0, calcManhattan(board)));

    cout << "AI is searching for the optimal path" << endl;

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        if (visited[current.board]) continue;
        visited[current.board] = true;

        board = current.board;
        emptyX = current.emptyX;
        emptyY = current.emptyY;
        stats.moves = current.g;
        drawBoard();
        Sleep(10);

        if (current.h == 0) {
            saveStats();
            cout << "AI solved the puzzle in " << current.g << " moves!\n";
            return;
        }

        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { 1, -1, 0, 0 };

        for (int i = 0; i < 4; i++) {
            int nX = current.emptyX + dx[i];
            int nY = current.emptyY + dy[i];

            if (nX >= 0 && nX < boardSize && nY >= 0 && nY < boardSize) {
                vector<vector<int>> nextBoard = current.board;
                swap(nextBoard[current.emptyX][current.emptyY], nextBoard[nX][nY]);

                if (visited.find(nextBoard) == visited.end()) {
                    pq.push(State(nextBoard, nX, nY, current.g + 1, calcManhattan(nextBoard)));
                }
            }
        }

        if (visited.size() > 100000) {
            cout << "\n[Error] AI got cooked by a puzzle" << endl;
            break;
        }
    }
}