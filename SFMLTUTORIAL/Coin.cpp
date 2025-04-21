#include "Coin.h"

// Define the static members
sf::SoundBuffer Coin::collectSoundBuffer;
sf::Sound Coin::collectSound;

Coin::Coin(const sf::Texture& texture, float x, float y)
    : collected(false) {
    coinSprite.setTexture(texture);
    coinSprite.setPosition(x, y);
    coinSprite.setScale(1.f, 1.f);  // Scale down if needed

}

//loading textures
void Coin::loadSound(const std::string& soundFile) {
    if (!collectSoundBuffer.loadFromFile(soundFile)) {
        std::cerr << "Error loading sound file: " << soundFile << std::endl;
    }
    collectSound.setBuffer(collectSoundBuffer);  // Linking the sound buffer to the sound object
}

void Coin::draw(sf::RenderWindow& window) {
    if (!collected)
        window.draw(coinSprite);
}

void Coin::collect() {
    if (!collected) {
        collected = true;
        collectSound.play();  // Play the collection sound when the coin is collected
    }
}

bool Coin::isCollected() const {
    return collected;
}

sf::FloatRect Coin::getBounds() const {
    return coinSprite.getGlobalBounds();
}

sf::Vector2f Coin::getPosition() const {
    return coinSprite.getPosition();
}
