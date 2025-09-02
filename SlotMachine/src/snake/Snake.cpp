#include <algorithm>
#include "Snake.h"
#include "Board.h"
#include "ncursesui.h"

void Snake::reset(const Board& board) {
    direction = Direction::Up;
    body.clear();
    body.push_back(board.getMid());
}

void Snake::setDirection(Direction newDirection) {
    if (newDirection == direction) return;
    if (length() == 1) {
        direction = newDirection;
    } else {
        switch (newDirection) {
            case Direction::Up:    if (direction != Direction::Down)   direction = Direction::Up;      break;
            case Direction::Right: if (direction != Direction::Left)   direction = Direction::Right;   break;
            case Direction::Down:  if (direction != Direction::Up)     direction = Direction::Down;    break;
            case Direction::Left:  if (direction != Direction::Right)  direction = Direction::Left;    break;
        }
    }
}

void Snake::move(bool grow) {
    auto newHead = getHead();
    switch (direction) {
        case Direction::Up:     --newHead.y; break;
        case Direction::Right:  ++newHead.x; break;
        case Direction::Down:   ++newHead.y; break;
        case Direction::Left:   --newHead.x; break;
    }
    body.insert(body.begin(), newHead);
    if (!grow && length() > initialLength)
        body.pop_back();
}

bool Snake::isSelfCollision() const {
    return std::find(body.begin() + 1, body.end(), getHead()) != body.end();
}

void Snake::draw(const NcursesUI& ui) const {
    ui.drawChars(body, 'O');
}