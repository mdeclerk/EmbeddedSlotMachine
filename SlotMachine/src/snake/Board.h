#pragma once

#include "Point.h"

class NcursesUI;

class Board {
public:
    void resize(int x, int y, int width, int height);
    bool isInside(Point2i p) const;

    Point2i getP1() const { return p1; }
    Point2i getP2() const { return p2; }

    int getWidth() const { return p2.x - p1.x; }
    int getHeight() const { return p2.y - p1.y; }

    Point2i getMid() const { return { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 }; }

    void draw(const NcursesUI& ui) const;

private:
    Point2i p1, p2;
};
