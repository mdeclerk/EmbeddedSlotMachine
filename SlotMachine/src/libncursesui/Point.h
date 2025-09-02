#pragma once

template<typename T>
struct Point2 {
    T x, y;

    Point2() : x {}, y {} { }
    Point2(T xy) : x(xy), y(xy) { }
    Point2(T x, T y) : x(x), y(y) { }
    Point2(const Point2<T>& p) : x(p.x), y(p.y) { }

    Point2& operator=(const Point2& p) { x = p.x; y = p.y; return *this;}

    Point2& operator+=(const Point2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Point2& operator-=(const Point2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    
    Point2 operator+(const Point2& rhs) const { return { x + rhs.x, y + rhs.y }; }
    Point2 operator-(const Point2& rhs) const { return { x - rhs.x, y - rhs.y }; }

    Point2 operator*(const T& rhs) const { return { x * rhs, y * rhs }; }
    Point2 operator/(const T& rhs) const { return { x / rhs, y / rhs }; }

    bool operator==(const Point2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Point2& rhs) const { return x != rhs.x || y != rhs.y; }
};

using Point2i = Point2<int>;