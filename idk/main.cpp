#include <SFML/Graphics.hpp>
#include "Boid.h"

void drawGrid(sf::RenderWindow& window) {
    sf::Color lineColor{ 55, 55, 55, 55 };  // White color for grid lines 
    sf::Color backgroundColor(sf::Color::White);  // Black color for background

    // Draw background
    window.clear(backgroundColor);

    // Draw vertical grid lines
    for (int x = 0; x <= 1920; x += 50) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(static_cast<float>(x), 0), lineColor),
            sf::Vertex(sf::Vector2f(static_cast<float>(x), 1080), lineColor)
        };
        window.draw(line, 2, sf::Lines);
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= 1080; y += 50) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, static_cast<float>(y)), lineColor),
            sf::Vertex(sf::Vector2f(1920, static_cast<float>(y)), lineColor)
        };
        window.draw(line, 2, sf::Lines);
    }
}

int main() {
    const int numBoids = 90;
    std::vector<Boid> boids;

    for (int i = 0; i < numBoids; ++i) {
        boids.emplace_back(rand() % 1920, rand() % 1080);
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Boids Simulation");

    // Simulation control variables
    bool paused = false;
    bool x10Speed = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Get the mouse cursor position
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        // Simulation controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            paused = !paused;
            sf::sleep(sf::milliseconds(200));  // Sleep to avoid multiple key presses
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            for (auto& boid : boids) {
                boid = Boid(rand() % 1920, rand() % 1080);
            }
            sf::sleep(sf::milliseconds(200));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            x10Speed = true;
        }
        else {
            x10Speed = false;
        }

        // Draw grid lines
        drawGrid(window);

        // Update and draw each boid
        for (auto& boid : boids) {
            if (!paused) {
                if (x10Speed) {
                    for (int i = 0; i < 10; ++i) {
                        boid.update(boids, sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
                    }
                }
                else {
                    boid.update(boids, sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
                }
            }
            boid.draw(window);
        }

        window.display();
    }

    return 0;
}
