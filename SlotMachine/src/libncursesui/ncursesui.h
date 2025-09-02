#pragma once

#include <string>
#include <vector>
#include "Point.h"

class NcursesUI {
public:
    enum class Key { None = 0, Up, Right, Down, Left, Esc, Enter, Space, P, Q };
    enum class Color { Default = 0, Red, Green, Blue, Yellow, Cyan, Magenta, White };

public:
    NcursesUI(int width = 0, int height = 0, int delay = 100);
    ~NcursesUI();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Point2i getSize() const { return { width, height }; }

    void clear() const;
    void refresh() const;

    void drawChar(Point2i p, char ch) const;
    void drawChars(const std::vector<Point2i>& points, char ch) const;
    void drawText(Point2i p, const std::string& text) const;
    void drawFrame(Point2i p1, Point2i p2) const;
 
    Key getKey() const;
    
    int showOptionsDialog(const std::string& title, const std::vector<std::string>& options) const;

private:
    int width, height;
    int delay;
    std::vector<std::pair<Color, Color>> colors;
};
