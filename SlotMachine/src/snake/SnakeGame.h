#pragma once

#include <string>

#include "ncursesui.h"
#include "Board.h"
#include "Snake.h"
#include "Food.h"

class SnakeGame {
public:
    void run();

private:
    std::string title;
    bool isRunning;
    bool shouldRestart;

    NcursesUI ui;

    Board board;
    Snake snake;
    Food food;

    int score;

private:
    void reset(int width = 60, int height = 25);
    void draw();
    void input();
    void update();
};
