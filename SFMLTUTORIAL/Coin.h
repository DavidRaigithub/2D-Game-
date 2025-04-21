#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Constants.h"
using namespace std;

class Coin {
private:
    sf::Sprite coinSprite;
    bool collected;

    static sf::SoundBuffer collectSoundBuffer;  // Static sound buffer shared by all coins
    static sf::Sound collectSound;              // Static sound object shared by all coins

public:
    // Constructor that initializes the coin with a texture and position
    Coin(const sf::Texture& texture, float x, float y);

    // Draw the coin to the window, only if it has not been collected
    void draw(sf::RenderWindow& window);

    // Mark the coin as collected and play the sound
    void collect();

    // Check if the coin has been collected
    bool isCollected() const;

    // Get the bounds of the coin (for collision detection)
    sf::FloatRect getBounds() const;

    // Get the position of the coin
    sf::Vector2f getPosition() const;

    // Static function to load the sound (called once at the beginning)
    static void loadSound(const std::string& soundFile);
};
