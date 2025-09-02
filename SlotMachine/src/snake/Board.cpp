#include "Board.h"
#include "ncursesui.h"

void Board::resize(int x, int y, int width, int height) {
    p1 = { x, y };
    p2 = { x + width, y + height };
}

bool Board::isInside(Point2i p) const {
    return p.x > p1.x && p.x < p2.x && p.y > p1.y && p.y < p2.y;
}

void Board::draw(const NcursesUI& ui) const {
    ui.drawFrame(p1, p2);
}