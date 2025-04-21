#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "SFML/Audio.hpp"
#include "GameTextManager.h"

// Enum to represent movement directions
enum direction { Down, Left, Right, Up }; // down=0, left=1, right=2, up=3

class Player : public Character {
public:
    Player(const std::string& texturePath, float scale = 1.0f, float speed = 90.0f);

    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window)override;

    void resetFrame();

    void storePosition();         // Stores the current position of the player
    void revertToLastPosition();  // Reverts the player back to the last stored position

    // For collision detection
    sf::FloatRect getBounds() const;

    // Getter and Setter for position
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const override;
    // Add this public method:

    direction getLastDirection() const { return lastDirection; }
    void setRow(int row);


    // Add getter for move count



private:
    sf::Sprite playerImage;   // The sprite of the player
    sf::Texture pTexture;     // The texture of the player
    sf::Vector2i source;      // The current frame (x, y) of the animation
    float speed;              // Speed of movement
    float frameCounter;       // Frame counter for animation
    float frameSpeed;         // Speed of animation frames
    direction lastDirection;  // Stores the last direction of movement

    sf::Vector2f velocity;    // Velocity for the player movement

    sf::Vector2f lastPosition;  // Stores the player's last position

    int row = 0;
    sf::SoundBuffer footstepBuffer;  // Footstep sound buffer
    sf::Sound footstepSound;         // Footstep sound



};
