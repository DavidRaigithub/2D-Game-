#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Character.h"
#include <vector>

enum class DecorationType {
    Tree,
    Shadow,
    Fence
};

class Decoration : public Character {
private:
    sf::Sprite sprite;
    bool collidable;
    DecorationType type;

    static sf::SoundBuffer decorationsound; //shared among all decorations
    sf::Sound decocollideSound;

public:
    // Constructor that auto-assigns sound based on decoration type
    Decoration(sf::Texture& texture, float x, float y, bool isCollidable, DecorationType decorationType)
        : collidable(isCollidable), type(decorationType)
    {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        
            if (decorationType == DecorationType::Fence) {
                sprite.setScale(1.5f, 1.5f); // scale as needed
            }
        // Load sound only if the decoration is a Tree
        if (decorationType == DecorationType::Tree) {
            if (!decorationsound.loadFromFile("assets/Sounds/decocollide.wav")) {
                std::cout << "Error loading tree collision sound!" << std::endl;
            }
            else {
                decocollideSound.setBuffer(decorationsound);
                decocollideSound.setVolume(100);
            }
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    
    bool isBlocking() const {
        return collidable;
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    DecorationType getType() const {
        return type;
    }

    void playCollisionSound() {
        // Only play the sound if it's a Tree and not already playing
        if (type == DecorationType::Tree && collidable && decocollideSound.getStatus() != sf::Sound::Playing) {
            decocollideSound.play();
        }
    }
};
