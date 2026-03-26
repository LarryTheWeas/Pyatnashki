#ifndef GAME_H
#define GAME_H

#include <vector>

struct Stats {
    int moves = 0;
};

struct State {
    std::vector<std::vector<int>> board; 
    int emptyX = 0, emptyY = 0, g = 0, h = 0;

    State(std::vector<std::vector<int>> b, int x, int y, int _g, int _h)
        : board(b), emptyX(x), emptyY(y), g(_g), h(_h) {
    }

    State() = default;

    bool operator>(const State& other) const {
        return (g + h) > (other.g + other.h);
    }
};;



void initBoard(int size);      
void drawBoard();               
bool makeMove(int val);         
void shuffle(int steps);        
void manualShuffle();           
bool isWin();                   
void saveStats();               
void solveAI();                 
int calcManhattan(const std::vector<std::vector<int>>& b); 

#endif
