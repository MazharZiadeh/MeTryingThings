#include "SFML/Graphics.hpp"
#include "Boid.h"
#include <iostream>
#include <cmath>
#include <random>

const float PI = 3.14159265359f;

Boid::Boid(float x, float y) : position(x, y) {
    // Initialize velocity with random values
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    velocity.x = distribution(generator);
    velocity.y = distribution(generator);

    // Normalize the velocity to ensure constant speed
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length != 0) {
        velocity.x /= length;
        velocity.y /= length;
    }

    // Set random color and size
    color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
    size = distribution(generator) * 5 + 5; // Random size between 5 and 10
}

void Boid::update(const std::vector<Boid>& boids, const sf::Vector2f& mousePosition) {
    sf::Vector2f alignment = getAlignment(boids);
    sf::Vector2f cohesion = getCohesion(boids);
    sf::Vector2f separation = getSeparation(boids);
    sf::Vector2f mouseAvoidance = getMouseAvoidance(mousePosition);

    // Adjust velocity based on the rules and mouse avoidance
    velocity += alignment + cohesion + separation + mouseAvoidance;

    // Normalize the velocity
    float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (length != 0) {
        velocity.x /= length;
        velocity.y /= length;
    }

    // Update position
    position += velocity;

    // Wrap around the screen edges
    if (position.x < 0) position.x = 1920;
    if (position.x > 1920) position.x = 0;
    if (position.y < 0) position.y = 1080;
    if (position.y > 1080) position.y = 0;

    // Update trail
    trail.push_back(position);
    if (trail.size() > 50) {
        trail.pop_front();
    }
}

void Boid::draw(sf::RenderWindow& window) const {
    // Draw trail
    for (const auto& trailPos : trail) {
        sf::CircleShape trailCircle(1);
        trailCircle.setFillColor(color);
        trailCircle.setPosition(trailPos.x, trailPos.y);
        window.draw(trailCircle);
    }

    // Draw the boid
    sf::CircleShape boidShape(size);
    boidShape.setFillColor(color);
    boidShape.setPosition(position.x - size / 2, position.y - size / 2);
    window.draw(boidShape);
}

sf::Vector2f Boid::getAlignment(const std::vector<Boid>& boids) const {
    sf::Vector2f align(0, 0);
    for (const auto& boid : boids) {
        align += boid.velocity;
    }
    return align / static_cast<float>(boids.size());
}

sf::Vector2f Boid::getCohesion(const std::vector<Boid>& boids) const {
    sf::Vector2f center(0, 0);
    for (const auto& boid : boids) {
        center += boid.position;
    }
    center /= static_cast<float>(boids.size());
    return (center - position) / 200.0f;  // Adjust the divisor for cohesion strength
}

sf::Vector2f Boid::getSeparation(const std::vector<Boid>& boids) const {
    sf::Vector2f separation(0, 0);
    for (const auto& boid : boids) {
        sf::Vector2f diff = position - boid.position;
        float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (distance > 0 && distance < 150.0f) {  // Adjust the threshold for separation distance
            separation += diff / (distance * distance);
        }
    }
    return separation;
}

sf::Vector2f Boid::getMouseAvoidance(const sf::Vector2f& mousePosition) const {
    sf::Vector2f avoidance(0, 0);
    sf::Vector2f diff = position - mousePosition;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // Adjust the threshold for mouse avoidance
    float avoidanceThreshold = 100.0f;
    if (distance > 0 && distance < avoidanceThreshold) {
        // Increase the strength of avoidance based on distance
        float strengthFactor = 10.0f / distance;
        avoidance += diff * strengthFactor * strengthFactor;
    }

    return avoidance;
}

void Boid::updateGroup(const std::vector<std::vector<Boid>>& clusters) {
    // Find the cluster that the current boid belongs to
    for (size_t i = 0; i < clusters.size(); ++i) {
        for (const Boid& clusterBoid : clusters[i]) {
            if (this == &clusterBoid) {
                // Update the color of the boid based on its cluster
                color = sf::Color(50 + i * 50, 50 + i * 50, 50 + i * 50);
                return;
            }
        }
    }
}
