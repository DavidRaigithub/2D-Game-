#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window, sf::Font& font)
        : window(window), font(font), showCredits(false) {

        // Load font
        if (!font.loadFromFile("assets/fonts/finaldraft.otf")) {
            std::cout << "Failed to load font!" << std::endl;
        }

        // Load click sound
        if (!clickSoundBuffer.loadFromFile("assets/Sounds/menusound.wav")) {
            std::cout << "Failed to load click sound!" << std::endl;
        }
        clickSound.setBuffer(clickSoundBuffer);

        // Load background image
        if (!backgroundTexture.loadFromFile("assets/menuimage.jpg")) {
            std::cout << "Failed to load background image!" << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Setup "Start" button
        startButton.setFont(font);
        startButton.setString("Start");
        startButton.setCharacterSize(48);
        startButton.setFillColor(sf::Color::White);
        startButton.setPosition(310.f, 250.f);

        // Setup "Exit" button
        exitButton.setFont(font);
        exitButton.setString("Exit");
        exitButton.setCharacterSize(48);
        exitButton.setFillColor(sf::Color::White);
        exitButton.setPosition(310.f, 300.f);

        // Setup "Credits" button
        creditsButton.setFont(font);
        creditsButton.setString("Credits");
        creditsButton.setCharacterSize(48);
        creditsButton.setFillColor(sf::Color::White);
        creditsButton.setPosition(310.f, 350.f);

        // Setup "Adventure Game" text with different color
        adventureGameText.setFont(font);
        adventureGameText.setString("  Survival Game");
        adventureGameText.setCharacterSize(72);
        adventureGameText.setFillColor(sf::Color::White);
        adventureGameText.setPosition(110.f, 100.f);

        // Setup credits text (hidden by default)
        creditsText.setFont(font);
        creditsText.setString("Credits:\nGame Developed by:David Rai \nMentor: Mr. Ashish Acharya \nSpecial Thanks to : SFML Community\n");
        creditsText.setCharacterSize(20);
        creditsText.setFillColor(sf::Color::Yellow);
        creditsText.setPosition(400.f, 450.f);
    }

    void draw() {
        window.clear();
        window.draw(backgroundSprite); // Draw background first
        window.draw(adventureGameText); // Draw "Adventure Game" text
        window.draw(startButton);
        window.draw(exitButton);
        window.draw(creditsButton);    // Drawing "Credits" button

        // If credits is true, draw the credits text
        if (showCredits) {
            window.draw(creditsText);
        }

        window.display();
    }

    void handleInput(bool& gameRunning) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Check if the mouse is hovering over the buttons and change the button color
        if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            startButton.setFillColor(sf::Color::Red); // Change color on hover
        }
        else {
            startButton.setFillColor(sf::Color::White); // Default color
        }

        if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            exitButton.setFillColor(sf::Color::Red); // Change color on hover
        }
        else {
            exitButton.setFillColor(sf::Color::White); // Default color
        }

        if (creditsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            creditsButton.setFillColor(sf::Color::Red); // Change color on hover
        }
        else {
            creditsButton.setFillColor(sf::Color::White); // Default color
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (startButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        clickSound.play();
                        clickSound.setVolume(80);
                        sf::sleep(sf::seconds(0.5));
                        gameRunning = true;
                    }
                    if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        clickSound.play();
                        clickSound.setVolume(80);
                        sf::sleep(sf::seconds(0.5));
                        window.close();
                    }
                    if (creditsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        clickSound.play();
                        clickSound.setVolume(80);
                        sf::sleep(sf::seconds(0.5));
                        showCredits = !showCredits;  // Toggle showing the credits
                    }
                }
            }
        }
    }

private:
    sf::RenderWindow& window;
    sf::Font& font;
    sf::Text startButton;
    sf::Text exitButton;
    sf::Text creditsButton;    // Button for credits
    sf::Text adventureGameText;  // Text for "Adventure Game"
    sf::Text creditsText;        // Text for displaying the credits
    sf::SoundBuffer clickSoundBuffer;
    sf::Sound clickSound;

    sf::Texture backgroundTexture; // Background texture
    sf::Sprite backgroundSprite;   // Sprite to draw the background
    bool showCredits;  // Boolean to track whether to show the credits
};

