#include <SFML/Graphics.hpp>
#include "point.h"
#include <fstream>
#include "ransac.h"
#include <iostream>

std::vector<Point> readCSV(const std::string& filename)
{
    std::vector<Point> points;
    std::string line;
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            int delim_index = line.find_first_of(';');
            float x = atof(line.substr(0, delim_index).c_str());
            float y = atof(line.substr(delim_index + 1).c_str());
            points.push_back(Point(x, y));
        }
        myfile.close();
    }
    return points;
}

int main()
{
    const int gameWidth = 550;
    const int gameHeight = 550;
    const int dotSize = 1;
    const float gapThreshold = 30;
    const RANSAC_Params params = {5, 15, 10, 20, 100};

    srand(time(NULL));

    std::vector<Point> points = readCSV("data.csv");

    std::vector<sf::CircleShape> drawPoints;
    for (std::size_t i = 0; i < points.size(); i++)
    {
        sf::CircleShape circle(dotSize);
        circle.setPosition(points.at(i).x - dotSize, points.at(i).y - dotSize);
        circle.setFillColor(sf::Color::Black);
        drawPoints.push_back(circle);
    }

    std::vector<sf::VertexArray> lines;

    // Create the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "RANSAC",
                            sf::Style::Titlebar | sf::Style::Close, settings);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                lines.clear();
                std::vector<Line> result = RANSAC(points, params, gapThreshold);
                for (std::size_t i = 0; i < result.size(); i++)
                {
                    lines.push_back(sf::VertexArray(sf::Lines, 2));
                    lines[i][0] = sf::Vector2f(result[i].start.x, result[i].start.y);
                    lines[i][1] = sf::Vector2f(result[i].end.x, result[i].end.y);
                    lines[i][0].color = sf::Color::Red;
                    lines[i][1].color = sf::Color::Red;
                }
            }
        }

        window.clear(sf::Color::White);

        for (std::size_t i = 0; i < drawPoints.size(); i++)
        {
            window.draw(drawPoints.at(i));
        }

        for (int i = 0; i < lines.size(); i++)
        {
            window.draw(lines.at(i));
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
