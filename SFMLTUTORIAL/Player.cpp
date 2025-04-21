#include "Player.h"
#include <iostream>

// Constructor to initialize the player
Player::Player(const std::string& texturePath, float scale, float speed) :
    source(3, Up), frameCounter(0), frameSpeed(100.f), lastDirection(Up), speed(150) {

    if (!pTexture.loadFromFile(texturePath)) {
        std::cout << "Player image not loaded..." << std::endl;
        throw std::runtime_error("Failed to load texture");
    }

    // Texture is 256x256, 4 rows, 4 columns (assuming player animation is in 4x4 grid)
    sf::Vector2u textureSize = pTexture.getSize();
    textureSize.x = textureSize.x / 4; // 64 pixels per frame (256 / 4)
    textureSize.y = textureSize.y / 4; // 64 pixels per frame (256 / 4)

    if (textureSize.x != 64 || textureSize.y != 64) {
        std::cout << "Texture dimensions don't match expected frame sizes!" << std::endl;
        throw std::runtime_error("Invalid texture dimensions");
    }

    playerImage.setTexture(pTexture);
    playerImage.setScale(scale, scale);
    playerImage.setPosition(2, 0);  // Initial position (can be adjusted)

    // Load the footstep sound
    if (!footstepBuffer.loadFromFile("assets/Sounds/singlefoot.wav")) {
        std::cout << "Failed to load footstep sound." << std::endl;
        throw std::runtime_error("Failed to load footstep sound");
    }
    footstepSound.setBuffer(footstepBuffer);
    footstepSound.setVolume(10);

}

void Player::handleInput(float deltaTime) {
    bool isMoving = false;  // Flag to check if player is moving

    // Handle movement input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        source.y = Up;
        playerImage.move(0, -speed * deltaTime);
        lastDirection = Up;
        isMoving = true;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        source.y = Down;
        playerImage.move(0, speed * deltaTime);
        lastDirection = Down;
        isMoving = true;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        source.y = Right;
        playerImage.move(speed * deltaTime, 0);
        lastDirection = Right;
        isMoving = true;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        source.y = Left;
        playerImage.move(-speed * deltaTime, 0);
        lastDirection = Left;
        isMoving = true;

    }

    // Prevent player from moving outside the window
    sf::FloatRect bounds = playerImage.getGlobalBounds();
    sf::Vector2f pos = playerImage.getPosition();

    // Clamping the player position inside the window
    float windowWidth = 900;
    float windowHeight = 700;

    if (bounds.left < 0) pos.x = 0;
    if (bounds.top < 0) pos.y = 0;
    if (bounds.left + bounds.width > windowWidth) pos.x = windowWidth - bounds.width;
    if (bounds.top + bounds.height > windowHeight) pos.y = windowHeight - bounds.height;

    playerImage.setPosition(pos);

    // Play footstep sound if moving and the sound is not already playing
    if (isMoving && footstepSound.getStatus() != sf::Sound::Playing) {
        footstepSound.play();
        
    }

    // If no movement keys are pressed, set idle animation
    if (!isMoving) {
        source.y = lastDirection;
        source.x = 0;  // Reset to the idle frame
        footstepSound.stop();  // Stop footstep sound when idle
    }
    
}
void Player::update(float deltaTime) {
    frameCounter += deltaTime * 1000;

    // Animate the player (loop through frames)
    if (frameCounter >= frameSpeed) {
        frameCounter = 0;
        if (source.x < 3) {
            source.x++;  // Move to the next frame
        }
        else {
            source.x = 0;  // Reset to the first frame
        }
    }

    // Set texture rect for animation based on current frame and direction
    playerImage.setTextureRect(sf::IntRect(source.x * 64, source.y * 64, 64, 64));
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(playerImage);  // Draw the player sprite
}

// Reset the animation frame
void Player::resetFrame() {
    source.x = 0;
}

// Store the current position of the player
void Player::storePosition() {
    lastPosition = playerImage.getPosition();
}

// Revert the player back to the last stored position
void Player::revertToLastPosition() {
    playerImage.setPosition(lastPosition);
}

// Return the player's bounding box for collision detection
sf::FloatRect Player::getBounds() const {
    return playerImage.getGlobalBounds();
}

// Set the position of the player
void Player::setPosition(sf::Vector2f pos) {
    playerImage.setPosition(pos);
}

// Get the current position of the player
sf::Vector2f Player::getPosition() const {
    return playerImage.getPosition();
}

void Player::setRow(int newRow) {
    row = newRow;
    playerImage.setTextureRect(sf::IntRect(source.x * 64, source.y * 64, 64, 64));
    // If you're using a sprite sheet, also update the texture rect here:
}
