#include <array>
#include <vector>
#include "Tetromino.h"
#include "Board.h"
#include "ncursesui.h"
#include "MyRand.h"

// Each tetromino shape: rotations are arrays of Points relative to a local origin (0,0)
static const std::vector<std::vector<std::vector<Point2i>>> TetrominoShapes {{
    // I
    {{{0,1},{1,1},{2,1},{3,1}}, {{2,0},{2,1},{2,2},{2,3}}},
    // O
    {{{1,0},{2,0},{1,1},{2,1}}},
    // T
    {{{1,0},{0,1},{1,1},{2,1}}, {{1,0},{1,1},{2,1},{1,2}}, {{0,1},{1,1},{2,1},{1,2}}, {{1,0},{0,1},{1,1},{1,2}}},
    // S
    {{{1,0},{2,0},{0,1},{1,1}}, {{1,0},{1,1},{2,1},{2,2}}},
    // Z
    {{{0,0},{1,0},{1,1},{2,1}}, {{2,0},{1,1},{2,1},{1,2}}},
    // J
    {{{0,0},{0,1},{1,1},{2,1}}, {{1,0},{2,0},{1,1},{1,2}}, {{0,1},{1,1},{2,1},{2,2}}, {{1,0},{1,1},{0,2},{1,2}}},
    // L
    {{{2,0},{0,1},{1,1},{2,1}}, {{1,0},{1,1},{1,2},{2,2}}, {{0,1},{1,1},{2,1},{0,2}}, {{0,0},{1,0},{1,1},{1,2}}}
}};


static Tetromino::TetrominoType randomTetrominoType() {
    static bool seeded = false;
    if (!seeded) {
        MyRand::seed();
        seeded = true;
    }
    return static_cast<Tetromino::TetrominoType>(MyRand::randomInt(0, TetrominoShapes.size() - 1));
}

Tetromino::Tetromino(Point2i pos)
    : type(randomTetrominoType())
    , pos(pos)
    , rotation(0) { }

const std::vector<Point2i>& Tetromino::getBlocks() const {
    const auto& shape = TetrominoShapes[static_cast<int>(type)];
    return shape[rotation];
}

void Tetromino::move(Point2i delta) {
    pos += delta;
}

void Tetromino::rotate(bool reverse) {
    int n = TetrominoShapes[static_cast<int>(type)].size();
    if (reverse) {
        rotation = (rotation + n - 1) % n;
    } else {
        rotation = (rotation + 1) % n;
    }
}

void Tetromino::draw(const NcursesUI& ui, const Board& board) const {
    for (const auto& block : getBlocks()) {
        auto p = (board.getPos() + 1) + pos + block;
        ui.drawChar(p, 'O');
    }
}