#ifndef LINE_H
#define LINE_H

#include "point.h"

class Line
{
    public:
    Point start;
    Point end;
    float a;
    float b;
    bool vertical;
    Line(Point, Point);
};

#endif // LINE_H
