#include "Point.h"
#include "SnakeGame.h"

void SnakeGame::reset(int w, int h) {
    isRunning = true;
    shouldRestart = false;

    int x = (ui.getWidth() - w) / 2;
    int y = (ui.getHeight() - h) / 2;
    board.resize(x, y, w, h);

    snake.reset(board);

    food.reset(board);
    food.generate(board, snake);

    score = 0;
}

void SnakeGame::draw() {
    ui.clear();
    
    ui.drawText(board.getP1() + Point2i(board.getWidth() + 1, 1), "Score:");
    ui.drawText(board.getP1() + Point2i(board.getWidth() + 2, 3), std::to_string(score));
    
    board.draw(ui);
    snake.draw(ui);
    food.draw(ui);
    
    ui.refresh();
}

void SnakeGame::input() {
    switch (ui.getKey()) {
        case NcursesUI::Key::Up:    snake.setDirection(Snake::Direction::Up); break;
        case NcursesUI::Key::Right: snake.setDirection(Snake::Direction::Right); break;
        case NcursesUI::Key::Down:  snake.setDirection(Snake::Direction::Down); break;
        case NcursesUI::Key::Left:  snake.setDirection(Snake::Direction::Left); break;
        
        case NcursesUI::Key::P:
        case NcursesUI::Key::Q:
        case NcursesUI::Key::Esc:  
            switch (ui.showOptionsDialog("Menu", { "Continue", "Restart", "Quit" })) {
                case 1: 
                    shouldRestart = true;  
                    isRunning = false; 
                    break;
                case 2: 
                    shouldRestart = false; 
                    isRunning = false; 
                    break;
            }
            break;

        default: break;
    }
}

void SnakeGame::update() {
    if (food.checkCollisionAndConsume(board, snake)) {
        snake.move(true);
        ++score;
    } else
        snake.move();

    if (!board.isInside(snake.getHead()) || snake.isSelfCollision()) {
        std::string title = "Game Over! Final score: " + std::to_string(score); 
        std::vector<std::string> options = { "Restart", "Quit" };
        shouldRestart = (ui.showOptionsDialog(title, options) == 0);
        isRunning = false;
    }
}

void SnakeGame::run() {
    do {
        reset();
        while (isRunning) {
            draw();
            input();
            update();
        }
    } while (shouldRestart);
}

int main() {
    SnakeGame().run();
}