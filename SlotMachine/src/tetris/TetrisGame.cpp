#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "TetrisGame.h"
#include "Tetromino.h"
#include "ncursesui.h"
#include "Point.h"

TetrisGame::TetrisGame(int boardW, int boardH)
    : ui(0, 0, 50)
    , board({}, {boardW, boardH})
    , current(currentInitPos())
    , preview(previewInitPos())
    , gameOver(false)
    , shouldRestart(false)
    , lines(0)
    , fallDelay(10)
    , tick(0) { }

Point2i TetrisGame::currentInitPos() {
    return {board.getSize().x/2-2, 0};
}

Point2i TetrisGame::previewInitPos() {
    return {board.getSize().x+3, 2};
}

void TetrisGame::reset() {
    board.reset(ui);
    current = Tetromino(currentInitPos());
    preview = Tetromino(previewInitPos());
    gameOver = false;
    shouldRestart = false;
    lines = 0;
    fallDelay = 10;
    tick = 0;
}

void TetrisGame::draw() {
    ui.clear();
    
    board.draw(ui);
    current.draw(ui, board);

    ui.drawText(board.getPos() + Point2i(board.getSize().x + 3, 1), "Next:");
    preview.draw(ui, board);

    ui.drawText(board.getPos() + Point2i(board.getSize().x + 3, 8), "Lines:");
    ui.drawText(board.getPos() + Point2i(board.getSize().x + 4, 10), std::to_string(lines));

    ui.refresh();
}

void TetrisGame::input() {
    auto key = ui.getKey();
    if (key == NcursesUI::Key::None) 
        return;

    auto temp = current;
    switch (key) {
        case NcursesUI::Key::Left:
            temp.move({-1, 0});
            if (board.canPlace(temp)) 
                current = temp;
            break;
        case NcursesUI::Key::Right:
            temp.move({1, 0});
            if (board.canPlace(temp)) 
                current = temp;
            break;
        case NcursesUI::Key::Down:
            temp.move({0, 1});
            if (board.canPlace(temp)) 
                current = temp;
            break;
        case NcursesUI::Key::Up:
            current.rotate();
            if (!board.canPlace(current))
                current.rotate(true);
            break;
        case NcursesUI::Key::P:
        case NcursesUI::Key::Q:
        case NcursesUI::Key::Esc:
            switch (ui.showOptionsDialog("Menu", { "Continue", "Restart", "Quit" })) {
                case 1: 
                    shouldRestart = true;  
                    gameOver = true; 
                    break;
                case 2: 
                    shouldRestart = false; 
                    gameOver = true; 
                    break;
            }
            break;
        default:
            break;
    }
}

void TetrisGame::update() {
    if (++tick % fallDelay != 0) 
        return;

    if (board.canPlace(current, {0,1})) {
        current.move();
    } else {
        board.place(current);
        lines += board.clearLines();

        current = preview;
        current.setPos(currentInitPos());

        preview = Tetromino(previewInitPos());

        if (!board.canPlace(current)) {
            std::string title = "Game Over! Final score: " + std::to_string(lines); 
            std::vector<std::string> options = { "Restart", "Quit" };
            shouldRestart = (ui.showOptionsDialog(title, options) == 0);
            gameOver = true;
        }
    }
}

void TetrisGame::run() {
    do {
        reset();
        while (!gameOver) {
            draw();
            input();
            update();

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } while (shouldRestart);
}

int main() {
    TetrisGame(10, 20).run();
}
