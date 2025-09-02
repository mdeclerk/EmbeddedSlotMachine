#include <algorithm>
#include <ranges>
#include "ncursesui.h"
#include "MyRand.h"
#include "Point.h"
#include "Board.h"
#include "Snake.h"
#include "Food.h"

static Point2i randomPoint(const Board& board) {
    static bool seeded = false;
    if (!seeded) {
        MyRand::seed();
        seeded = true;
    }
    int x = MyRand::randomInt(board.getP1().x + 1, board.getP2().x - 2);
    int y = MyRand::randomInt(board.getP1().y + 1, board.getP2().y - 2);
    return {x, y};
}

void Food::reset(const Board& board) {
    itemCount = board.getWidth() * board.getHeight() * 0.001;
    itemCount = itemCount > 3 ? itemCount : 3;
    items.clear();
}

void Food::generate(const Board& board, const Snake& snake) {
    Point2i p;
    for (int i = items.size(); i < itemCount; ++i) {
        do {
            p = randomPoint(board);
        } while (
            std::ranges::find(items, p) != items.end() ||
            std::ranges::find(snake.getBody(), p) != snake.getBody().end()
        );
        items.push_back(p);
    }
}

bool Food::checkCollisionAndConsume(const Board& board, const Snake& snake) {
    auto it = std::ranges::find(items, snake.getHead());
    if (it != items.end()) {
        items.erase(it);
        generate(board, snake);
        return true;
    }
    return false;
}

void Food::draw(const NcursesUI& ui) const {
    ui.drawChars(items, 'F');
}