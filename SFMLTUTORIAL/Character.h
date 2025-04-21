#pragma once
#include <SFML/Graphics.hpp>

// for inheriting to player and enemy
class Character {
public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::Vector2f getPosition() const = 0;
    virtual ~Character() = default;
};
