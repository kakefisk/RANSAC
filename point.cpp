#include "point.h"
#include <math.h>
#include <stdlib.h>
#include "line.h"

Point::Point(float _x, float _y) : x(_x), y(_y) {}

Point Point::operator+(const Point& b) const
{
    return Point(x + b.x, y + b.y);
}

Point Point::operator-(const Point& b) const
{
    return Point(x - b.x, y - b.y);
}

float Point::distanceTo(const Point& b) const
{
    return sqrt((b.x - x)*(b.x - x) + (b.y - y)*(b.y - y));
}

float Point::distanceTo(const Line& line) const
{
    return abs(-line.a*x + y - line.b)/sqrt(line.a*line.a + 1);
}
