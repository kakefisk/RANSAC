#ifndef RANSAC_H
#define RANSAC_H

#include <vector>
#include "point.h"
#include "line.h"

struct RANSAC_Params
{
    float d; // Inlier threshold
    int T; // Minimum points
    int N; // Points left for exit
    int Lnr; // Number of lines for exit
    int M; // Number of attempts for exit
};

std::vector<Line> RANSAC(std::vector<Point> points, RANSAC_Params params, float gapThreshold);
Line random_line(const std::vector<Point>& points);

#endif // RANSAC_H
