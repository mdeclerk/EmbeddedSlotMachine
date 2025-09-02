#pragma once

#include <vector>
#include "ncursesui.h"
#include "Board.h"

class SpaceInvadersGame {
public:
    SpaceInvadersGame(int boardWidth = 50, int boardHeight = 25);
    void run();

private:
    NcursesUI ui;

    Board board;
    Point2i player;
    std::vector<Point2i> playerBullets;
    std::vector<Point2i> enemies;
    std::vector<Point2i> enemyBullets;
    
    int enemyDirection;

    bool gameOver;
    bool shouldRestart;
    int tick;
    int score;

private:
    void reset();
    void draw();
    void input();
    void update();

    Point2i getPlayerStartPos(int w, int h);
};

