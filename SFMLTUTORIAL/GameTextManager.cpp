#include "GameTextManager.h"
#include <iostream>
using namespace std;

GameTextManager::GameTextManager(sf::Font& font) {
    // Existing initialization for other texts
    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(10.f, 10.f);

    nextLevelText.setFont(font);
    nextLevelText.setCharacterSize(24);
    nextLevelText.setFillColor(sf::Color::White);
    nextLevelText.setPosition(200.f, 250.f);
    nextLevelText.setString("Press N to go to next level");

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(1.f, 540.f);

    completionText.setFont(font);
    completionText.setCharacterSize(36);
    completionText.setFillColor(sf::Color::Yellow);
    completionText.setStyle(sf::Text::Bold);
    completionText.setPosition(200.f, 250.f);
    completionText.setString("Congratulations!!\nGame Completed!");

    saveLoadText.setFont(font);
    saveLoadText.setCharacterSize(15);
    saveLoadText.setFillColor(sf::Color::Red);
    saveLoadText.setPosition(490.f, 580.f);
    saveLoadText.setString("Press R to save and L to load the game");

    // Load completion sound
    if (!completionSoundBuffer.loadFromFile("assets/Sounds/winsound.ogg")) {
        cout << "Failed to load completion sound!" << endl;
    }
    else {
        completionSound.setBuffer(completionSoundBuffer);
    }

    // Load game over sound
    if (!gameOverSoundBuffer.loadFromFile("assets/Sounds/Defeatsound.ogg")) {
        cout << "Failed to load game over sound!" << endl;
    }
    else {
        gameOverSound.setBuffer(gameOverSoundBuffer);
    }

    // Initializing Game Over text
    GameoverText.setFont(font);
    GameoverText.setString("Game Over");
    GameoverText.setCharacterSize(50);
    GameoverText.setFillColor(sf::Color::Red);
    GameoverText.setPosition(250.f, 250.f);  // Center position, can adjust

  
}

void GameTextManager::updateLevel(int level) {
    levelText.setString("Current Level: " + std::to_string(level));
}

void GameTextManager::updateScore(int score) {
    scoreText.setString("Score: " + std::to_string(score));
}

void GameTextManager::updateMoveCount(int moveCount) {
    // Update the move count text
    moveCountText.setString("Moves: " + std::to_string(moveCount));  // Convert int to string
}


void GameTextManager::drawMoveCount(sf::RenderWindow& window) {
    window.draw(moveCountText);
}
void GameTextManager::draw(sf::RenderWindow& window) {
    window.draw(levelText);
    window.draw(scoreText);
    window.draw(saveLoadText);

    

    // draws if the flag is true
    if (showNextLevelText) {
        window.draw(nextLevelText);
    }
}

void GameTextManager::drawCompletionText(sf::RenderWindow& window) {
    window.draw(completionText);
}

void GameTextManager::drawSaveLoadText(sf::RenderWindow& window) {
    window.draw(saveLoadText);
}

//  method to draw the "Game Over" text
void GameTextManager::drawGameOverText(sf::RenderWindow& window) const {
    window.draw(GameoverText);  // Simply draw the game over text
}

// Play completion sound once
void GameTextManager::playCompletionSound() {
    if (!hasPlayedCompletionSound) {
        completionSound.play();
        hasPlayedCompletionSound = true;
    }
}

// Play the game over sound once
void GameTextManager::playGameOverSound() {
    if (!hasPlayedGameOverSound) {
        gameOverSound.play();
        hasPlayedGameOverSound = true;
    }
}

void GameTextManager::setNextLevelVisible(bool visible) {
    showNextLevelText = visible;
}

// Get the duration of the game over sound
sf::Time GameTextManager::getGameOverSoundDuration() const {
    return gameOverSound.getBuffer()->getDuration();
}
