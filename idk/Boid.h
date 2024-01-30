#pragma once

#include <SFML/Graphics.hpp>
#include <deque>  // Include this line for std::deque

class Boid {
public:
    Boid(float x, float y);

    void update(const std::vector<Boid>& boids, const sf::Vector2f& mousePosition);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getAlignment(const std::vector<Boid>& boids) const;
    sf::Vector2f getCohesion(const std::vector<Boid>& boids) const;
    sf::Vector2f getSeparation(const std::vector<Boid>& boids) const;
    sf::Vector2f getMouseAvoidance(const sf::Vector2f& mousePosition) const;

    void updateGroup(const std::vector<std::vector<Boid>>& clusters);

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;  // Add this line to define the color member variable
    float size;
    std::deque<sf::Vector2f> trail;
};
