#pragma once

#include <vector>
#include "Point.h"

class NcursesUI;
class Board;
class Snake;

class Food {
public:
    void reset(const Board& board);
    void generate(const Board& board, const Snake& snake);
    bool checkCollisionAndConsume(const Board& board,const Snake& snake);

    void draw(const NcursesUI& ui) const;

private:
    int itemCount;
    std::vector<Point2i> items;
};