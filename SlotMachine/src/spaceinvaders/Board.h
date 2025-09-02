#pragma once

#include "Point.h"
#include "ncursesui.h"

class Board {
public:
    Board(Point2i pos, Point2i size)
        : pos(pos), size(size) { }

    void draw(const NcursesUI& ui) const { 
        ui.drawFrame(pos, pos + size); 
    }

    bool isInside(Point2i p) const {
        return p.x > pos.x && p.x < pos.x + size.x - 1 &&
               p.y > pos.y && p.y < pos.y + size.y - 1;
    }

    Point2i getPos() const { return pos; }
    void setPos(Point2i p) { pos = p; }
    Point2i getSize() const { return size; }
    void setSize(Point2i s) { size = s; }

    Point2i getPlayerStartPos() const { 
        return { pos.x + size.x / 2, pos.y + size.y - 2 };
    }

private:
    Point2i pos, size;
};
