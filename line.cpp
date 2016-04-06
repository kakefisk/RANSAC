#include "line.h"

Line::Line(Point _start, Point _end) : start(_start), end(_end)
{
    float dx = end.x - start.x;
    if (dx == 0)
    {
        a = 10000;
    }
    else
    {
        a = (end.y - start.y)/dx;
    }
    b = start.y - a*start.x;
}
