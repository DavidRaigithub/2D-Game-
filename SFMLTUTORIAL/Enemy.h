#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>  // Include for sound handling
#include <iostream>
#include "Character.h"

class AnimatedEnemy : public Character {
public:
    sf::Texture texture;
    sf::Sprite sprite;
    int frameWidth = 64;
    int frameHeight = 64;
    int columns = 6;    // Number of frames in each row
    int rows = 4;       // Number of rows in the sprite sheet
    int currentFrame = 0;
    int currentRow = 3; // Start moving right (row 3)
    float animationTime = 0.1f; // Time per frame in animation
    float elapsedTime = 0.f;
    float speed = 50.f; // Speed of movement
    sf::Vector2f velocity = { 100.f, 0.f }; // Initial horizontal movement (this will change)

    // Sound members
    sf::SoundBuffer collisionSoundBuffer;  // Sound buffer for collision
    sf::Sound collisionSound;              // Sound object to play collision sound
    sf::SoundBuffer followSoundBuffer;     // Sound buffer for following sound
    sf::Sound followSound;                 // Sound object to play follow sound

    // Constructor to load the texture, set up the sprite, and load the sound
    AnimatedEnemy(const std::string& spriteSheetPath, const std::string& collisionSoundPath, const std::string& followSoundPath) {
        if (!texture.loadFromFile(spriteSheetPath)) {
            std::cout << "Failed to load enemy sprite sheet!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight)); // Initial texture rect
        sprite.setPosition(200.f, 200.f);  // Initial position
        sprite.setScale(1.5f, 1.5f);       // Scale the sprite

        // Load the collision sound
        if (!collisionSoundBuffer.loadFromFile(collisionSoundPath)) {
            std::cout << "Failed to load collision sound!" << std::endl;
        }
        collisionSound.setBuffer(collisionSoundBuffer);

        // Load the follow sound
        if (!followSoundBuffer.loadFromFile(followSoundPath)) {
            std::cout << "Failed to load follow sound!" << std::endl;
        }
        followSound.setBuffer(followSoundBuffer);
    }

    // Update animation based on elapsed time
    void update(float deltaTime) {
        elapsedTime += deltaTime;
        if (elapsedTime >= animationTime) {
            elapsedTime = 0.f;
            currentFrame = (currentFrame + 1) % columns;

            sprite.setTextureRect(sf::IntRect(
                currentFrame * frameWidth,
                currentRow * frameHeight,
                frameWidth,
                frameHeight
            ));
        }
    }

    // Move the enemy toward the player
    void move(float deltaTime, const sf::Vector2f& playerPosition) {
        // Calculate the direction vector from the enemy to the player
        sf::Vector2f direction = playerPosition - sprite.getPosition();

        // Normalize the direction to get a unit vector
        float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (magnitude != 0) {
            direction /= magnitude; // Normalize the vector
        }

        // Update velocity to move the enemy toward the player
        velocity = direction * speed;

        // Move the enemy in the calculated direction
        sprite.move(velocity * deltaTime);

        // Play the following sound when the enemy is moving
        if (magnitude > 10.f) { // Only play sound if the enemy is moving towards the player
            if (followSound.getStatus() != sf::Sound::Playing) {
                followSound.play(); // Play sound only if not already playing
            }
        }
    }

    // Check collision with walls or boundaries (could be used for boundaries or obstacles)
    void checkCollisionWithWall(const sf::RectangleShape& wall) {
        if (sprite.getGlobalBounds().intersects(wall.getGlobalBounds())) {
            velocity.x *= -1; // Reverse direction on collision with wall

            // Switch to appropriate row for new direction
            if (velocity.x > 0) {
                setRow(3);  // Moving right
            }
            else {
                setRow(2);  // Moving left
            }
        }
    }

    // Play collision sound (to be called on player collision)
    void playCollisionSound() {
        collisionSound.play();
    }

    // Draw the enemy sprite to the window
    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    // Set the position of the sprite (used to reposition the enemy)
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    // Set the current row for animation (used to change animation direction)
    void setRow(int row) {
        if (row >= 0 && row < rows) {
            currentRow = row;
        }
    }

    // Get the global bounds of the enemy sprite (for collision checks)
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    // Get the current position of the sprite
    sf::Vector2f getPosition() const override {
        return sprite.getPosition();
    }

    // Get the current row of animation
    int getRow() const {
        return currentRow;
    }

    // Check for collision with another entity (like a player or projectile)
    bool checkCollisionWithEntity(const sf::FloatRect& otherBounds) const {
        return sprite.getGlobalBounds().intersects(otherBounds);
    }

    // Set the scale of the enemy sprite
    void setScale(float x, float y) {
        sprite.setScale(x, y);
    }
};
