#pragma once

#include <vector>
#include "Point.h"

class NcursesUI;
class Tetromino;

class Board {
public:
    Board(Point2i pos, Point2i size);

    Point2i getPos() const { return pos; }
    void setPos(Point2i p) { pos = p; }
    Point2i getSize() const { return size; }
    void setSize(Point2i s) { size = s; }

    void reset(const NcursesUI& ui);

    bool canPlace(const Tetromino& t, Point2i offset = {}) const;
    void place(const Tetromino& t);
    int clearLines();

    void draw(NcursesUI& ui) const;

private:
    Point2i pos;
    Point2i size;
    std::vector<std::vector<char>> grid;
};