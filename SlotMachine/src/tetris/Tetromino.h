#pragma once

#include "Point.h"

class NcursesUI;
class Board;

class Tetromino {
public:
    enum class TetrominoType { I, O, T, S, Z, J, L };

public:
    Tetromino(Point2i pos);

    const std::vector<Point2i>& getBlocks() const;
    
    Point2i getPos() const { return pos; }
    void setPos(Point2i p) { pos = p; }
    
    void move(Point2i delta = {0,1});
    void rotate(bool reverse = false);

    void draw(const NcursesUI& ui, const Board& board) const;
    
private:
    TetrominoType type;
    Point2i pos;
    int rotation;
};