#include <chrono>
#include <thread>
#include <algorithm>
#include "SpaceInvadersGame.h"
#include "MyRand.h"

SpaceInvadersGame::SpaceInvadersGame(int w, int h)
    : ui(0, 0, 50)
    , board(Point2i(0, 0), Point2i(w, h))
    , player(board.getPlayerStartPos())
    , playerBullets {}
    , enemies {}
    , enemyBullets {}
    , enemyDirection(1)
    , gameOver(false)
    , shouldRestart(false)
    , tick(0)
    , score(0) { }

void SpaceInvadersGame::reset() {
    board.setPos((ui.getSize() - board.getSize())/2);

    player = board.getPlayerStartPos();
    
    enemies.clear();
    for (int y = board.getPos().y + 2; y < board.getPos().y + board.getSize().y / 2 - 2; ++y) {
        for (int x = board.getPos().x + 4; x < board.getPos().x + board.getSize().x - 4; x += 6) {
            enemies.emplace_back(Point2i(x, y));
        }
    }

    playerBullets.clear();
    enemyBullets.clear();

    enemyDirection = 1;
    
    gameOver = false;
    shouldRestart = false;
    tick = 0;
    score = 0;
}

void SpaceInvadersGame::draw() {
    ui.clear();

    board.draw(ui);

    ui.drawChar(player, 'A');

    for (const auto& b : playerBullets)
        ui.drawChar(b, '^');

    for (const auto& e : enemies)
        ui.drawChar(e, tick % 4 == 0 ? 'W' : 'X');

    for (const auto& b : enemyBullets)
        ui.drawChar(b, '|');
        
    ui.drawText(board.getPos() + Point2i(board.getSize().x + 1, 1), "Score:");
    ui.drawText(board.getPos() + Point2i(board.getSize().x + 2, 3), std::to_string(score));

    ui.refresh();
}

void SpaceInvadersGame::input() {
    auto newPlayer = player;
    switch (ui.getKey()) {
        case NcursesUI::Key::Left:
            --newPlayer.x;
            if (board.isInside(newPlayer))
                player = newPlayer;
            break;
        case NcursesUI::Key::Right: 
            ++newPlayer.x;
            if (board.isInside(newPlayer))
                player = newPlayer;
            break;
        case NcursesUI::Key::Space: 
            playerBullets.emplace_back(player + Point2i(0, -1)); 
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

void SpaceInvadersGame::update() {
    ++tick;

    // update player bullets
    std::erase_if(playerBullets, [&](Point2i& b) {
        --b.y;

        if (!board.isInside(b))
            return true; // erase bullet if it left the board

        auto it = std::ranges::find_if(enemies, [&](const Point2i& e) { return b == e; });
        if (it != enemies.end()) {
            score += 1;
            enemies.erase(it); // erase enemy
            if (enemies.empty()) gameOver = true;
            return true; // erase bullet
        }

        return false;
    });

    // update enemy bullets
    std::erase_if(enemyBullets, [&](Point2i& b) {
        ++b.y;

        if (!board.isInside(b))
            return true; // erase bullet if it left the board

        if (player == b) {
            gameOver = true;
            return true; // erase bullet
        }

        return false;
    });

    // Enemy movement
    if (tick % 8 == 0) { // Move every 8 ticks
        bool hitEdge = false;
        for (const auto& e : enemies) {
            if ((enemyDirection == 1 && e.x >= board.getPos().x + board.getSize().x - 2) ||
                (enemyDirection == -1 && e.x <= board.getPos().x + 1)) {
                hitEdge = true;
                break;
            }
        }
        if (hitEdge) {
            enemyDirection *= -1;
            for (auto& e : enemies)
                e.y += 1; // Move down
        } else {
            for (auto& e : enemies)
                e.x += enemyDirection; // Move left/right
        }
    }

    // Enemy shooting
    if (MyRand::randomFloat01() < 0.1) {
        for (auto& e : enemies) {
            if (MyRand::randomFloat01() < 0.05) {
                enemyBullets.emplace_back(e + Point2i(0,1));
            }
        }
    }

    // Game over dialog
    if (gameOver) {
        std::string title = enemies.empty() ? "You won!": "You lost!";
        title += " Score: " + std::to_string(score);
        std::vector<std::string> options = { "Restart", "Quit" };
        shouldRestart = (ui.showOptionsDialog(title, options) == 0);
        gameOver = true;
    }
}

void SpaceInvadersGame::run() {
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
    MyRand::seed();
    SpaceInvadersGame().run();
}