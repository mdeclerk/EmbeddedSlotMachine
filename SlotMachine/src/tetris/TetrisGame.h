#pragma once

#include "ncursesui.h"
#include "Board.h"
#include "Tetromino.h"

class TetrisGame {
public:
    TetrisGame(int boardW, int boardH);
    void run();

private:
    NcursesUI ui;

    Board board;
    Tetromino current;
    Tetromino preview;

    bool gameOver;
    bool shouldRestart;
    int lines;
    int fallDelay;
    int tick;

private:
    Point2i currentInitPos();
    Point2i previewInitPos();

    void reset();
    void draw();
    void input();
    void update();
};
