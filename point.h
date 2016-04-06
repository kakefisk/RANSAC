#ifndef POINT_H
#define POINT_H

class Line;

class Point
{
    public:
    float x;
    float y;
    Point(float, float);
    Point operator+(const Point& b) const;
    Point operator-(const Point& b) const;
    float distanceTo(const Point& b) const;
    float distanceTo(const Line& line) const;
};

#endif // POINT_H
