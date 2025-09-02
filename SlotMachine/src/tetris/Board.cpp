#include <algorithm>
#include "Board.h"
#include "Tetromino.h"
#include "ncursesui.h"

Board::Board(Point2i pos, Point2i size) 
    : pos(pos)
    , size(size)
    , grid(size.y, std::vector<char>(size.x, ' ')) { }

void Board::reset(const NcursesUI& ui) {
    // center
    pos = (ui.getSize() - size) / 2 - 1;

    //clear grid
    for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
            grid[y][x] = ' ';
}

bool Board::canPlace(const Tetromino& t, Point2i offset) const {
    for (const auto& p : t.getBlocks()) {
        auto abs = t.getPos() + p + offset;
        if (abs.x < 0 || abs.x >= size.x || abs.y < 0 || abs.y >= size.y || grid[abs.y][abs.x] != ' ')
            return false;
    }
    return true;
}

void Board::place(const Tetromino& t) {
    for (const auto& p : t.getBlocks()) {
        auto abs = t.getPos() + p;
        if (abs.x >= 0 && abs.x < size.x && abs.y >= 0 && abs.y < size.y)
            grid[abs.y][abs.x] = 'X';
    }
}

int Board::clearLines() {
    int lines = 0;
    for (int y = size.y-1; y >= 0; --y) {
        if (std::all_of(grid[y].begin(), grid[y].end(), [](char c){ return c != ' '; })) {
            grid.erase(grid.begin() + y);
            grid.insert(grid.begin(), std::vector<char>(size.x, ' '));
            ++lines;
            ++y; // recheck the new row at this position
        }
    }
    return lines;
}

void Board::draw(NcursesUI& ui) const {
    // Draw frame
    ui.drawFrame(pos, pos + size + 2);

    // Draw blocks
    for (int y = 0; y < size.y; ++y)
        for (int x = 0; x < size.x; ++x)
            if (grid[y][x] != ' ')
                ui.drawChar(pos + Point2i(x, y) + 1, grid[y][x]);
}