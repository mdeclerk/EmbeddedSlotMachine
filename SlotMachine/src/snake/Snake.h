#pragma once

#include <vector>
#include "Point.h"

class NcursesUI;
class Board;

class Snake {
public:
    enum class Direction { Up, Right, Down, Left };

public:
    void reset(const Board& board);
    
    void setDirection(Direction newDirection);
    void move(bool grow = false);

    bool isSelfCollision() const;

    const Point2i& getHead() const { return body.front(); }
    const std::vector<Point2i>& getBody() const { return body; }
    int length() const { return static_cast<int>(body.size()); }

    void draw(const NcursesUI& ui) const;

private:
    int initialLength = 3;
    Direction direction = Direction::Up;
    std::vector<Point2i> body;
};