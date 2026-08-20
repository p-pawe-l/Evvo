#pragma once

#include <utility>

struct Vec2D {
    double x, y;
    Vec2D(double x, double y);

    Vec2D operator+(const Vec2D& other) const;
    Vec2D operator-(const Vec2D& other) const;
    Vec2D operator*(double s) const;
    double dot(const Vec2D& other) const;
    double length() const;
};