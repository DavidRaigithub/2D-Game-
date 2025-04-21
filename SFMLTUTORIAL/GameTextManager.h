#ifndef GAMETEXTMANAGER_H
#define GAMETEXTMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/Audio.hpp>


class GameTextManager {
public:
    GameTextManager(sf::Font& font);

    void updateLevel(int level);//method to show updated level
    void updateScore(int score);//method to show the updaed score
    void draw(sf::RenderWindow& window);//drawing window
    void drawCompletionText(sf::RenderWindow& window);
    void drawSaveLoadText(sf::RenderWindow& window);
    void playCompletionSound();
    void playGameOverSound();  //  method to play the game over sound
    sf::Time getGameOverSoundDuration() const;
    void drawGameOverText(sf::RenderWindow& window) const; // Method to draw Game Over text
    void setNextLevelVisible(bool visible);

    void updateMoveCount(int count);  // New method to update move count
    void drawMoveCount(sf::RenderWindow& window);  // New method to draw move count


private:
    sf::Text levelText;
    sf::Text nextLevelText;
    sf::Text scoreText;
    sf::Text completionText;
    sf::Text saveLoadText;
    sf::Text GameoverText;
    bool showNextLevelText = false; 

    sf::Text moveCountText;  // New member to display the move count text
    int moveCount = 0;       // New variable to store the move count

    sf::SoundBuffer completionSoundBuffer;
    sf::Sound completionSound;
    bool hasPlayedCompletionSound = false;

    sf::SoundBuffer gameOverSoundBuffer;   // Buffer for the game over sound
    sf::Sound gameOverSound;               // Sound object for the game over sound
    bool hasPlayedGameOverSound = false;   // Flag to track if the game over sound has played
};

#endif
