#include "ransac.h"
#include <stdlib.h>

int random_between(int min, int max)
{
    return rand()%(max-min + 1) + min;
}

Line random_line(const std::vector<Point>& points)
{
    int i1 = random_between(0, points.size() - 1);
    int i2;
    do
    {
        // Ensure the same point isn't picked twice
        i2 = random_between(0, points.size() - 1);
    } while (i2 == i1);
    return Line(points[i1], points[i2]);
}

std::vector<Point> LeastSquares(std::vector<Point> points, float& a, float &b)
{
    float x_mean, y_mean, numerator, denominator = 0;
    for (int i = 0; i < points.size(); i++)
    {
        x_mean += points[i].x;
        y_mean += points[i].y;
    }
    x_mean /= points.size();
    y_mean /= points.size();
    for (int i = 0; i < points.size(); i++)
    {
        numerator += (points[i].x - x_mean)*(points[i].y - y_mean);
        denominator += (points[i].x - x_mean)*(points[i].x - x_mean);
    }
    a = numerator/denominator;
    b = y_mean - a*x_mean;
    for (int i = 0; i < points.size(); i++)
    {
        // Calculates new y values after the linear regression
        points[i].y = a*points[i].x + b;
    }
    return points;
}

std::vector<Point> LeastSquares(std::vector<Point> points)
{
    float a, b;
    return LeastSquares(points, a, b);
}

std::vector<Line> RANSAC(std::vector<Point> points, RANSAC_Params params, float gapThreshold)
{
    std::vector<Line> result;
    for (int i = 0; i < params.M; i++)
    {
        Line line = random_line(points);
        std::vector<Point> inliers;

        int begin = -1, end;
        for (int i = 0; i < points.size(); i++)
        {
            if (points[i].distanceTo(line) <= params.d)
            {
                if (begin < 0)
                {
                    begin = i;
                }
                end = i;
                inliers.push_back(points[i]);
            }
        }

        if (inliers.size() >= params.T)
        {
            std::vector<Point> linearized_points = LeastSquares(inliers);
            std::vector<int> gapIndices;
            gapIndices.push_back(0);

            for (int i = 0; i < linearized_points.size() - 1; i++)
            {
                if (linearized_points[i].distanceTo(linearized_points[i + 1]) >= gapThreshold)
                {
                    // Pushes the index to the point after a gap
                    gapIndices.push_back(i + 1);
                }
            }

            if (gapIndices.size() <= 1)
            {
                // If there are no gaps, add the line directly
                result.push_back(Line(linearized_points.front(), linearized_points.back()));
                points.erase(points.begin() + begin, points.begin() + end);
            }
            else
            {
                for (int i = 0; i < gapIndices.size() - 1; i++)
                {
                    // If there are sufficient points on the line
                    if (gapIndices[i + 1] - gapIndices[i] >= params.T)
                    {
                        // Adds the line to the results
                        result.push_back(Line(linearized_points[gapIndices[i]], linearized_points[gapIndices[i + 1] - 1]));

                        // Remove the points from the main point list
                        points.erase(points.begin() + begin + gapIndices[i], points.begin() + begin + gapIndices[i + 1] - 1);
                    }
                }
            }
        }
        if (result.size() >= params.Lnr || points.size() <= params.N) break;
    }

    return result;
}
