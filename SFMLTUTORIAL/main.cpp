#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Decoration.h"
#include "MainMenu.h"
#include "Grid.h"
#include "GameTextManager.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

// Static sound buffer definition for collision 
sf::SoundBuffer Decoration::decorationsound;

int main()
{
    int playerhealth = 100;
    float maxhealth = 100;
    bool gameover = false;
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D SURVIVAL GAME", sf::Style::Close );


    //---------------------Loading Textures and Placing the position---------------------------------------//
    sf::Texture backgroundTexture, level3bgtexture;
    if (!backgroundTexture.loadFromFile("assets/Fields.png")) {
        cout << "Error loading background texture.\n";
        return -1;
    }
    if (!level3bgtexture.loadFromFile("assets/thirdbackground.png")) {
        cout << "Error loading background texture.\n";
        return -1;
    }
    sf::Sprite background(backgroundTexture);
    background.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );
    sf::Sprite level3bg(level3bgtexture);
    level3bg.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );


    std::vector<Decoration> decorations;
    sf::Texture treeTexture, shadowTexture,fencetexture;
    treeTexture.loadFromFile("assets/Tree.png");
    shadowTexture.loadFromFile("assets/shadow.png");
    fencetexture.loadFromFile("assets/fence1.png");

    decorations.emplace_back(treeTexture, 1* TILE_SIZE, 9* TILE_SIZE, true, DecorationType::Tree);
    decorations.emplace_back(shadowTexture, 19 * TILE_SIZE, 13 * TILE_SIZE, true, DecorationType::Shadow);
    decorations.emplace_back(treeTexture, 20 * TILE_SIZE, 12* TILE_SIZE, true, DecorationType::Tree);



    sf::Texture wallTexture;
    if (!wallTexture.loadFromFile("assets/Stone.png")) {
        cout << "Error loading wall texture.\n";
        return -1;
    }

    sf::Texture coinTexture;
    if (!coinTexture.loadFromFile("assets/coin/Coin.png")) {
        cout << "Error loading coin texture.\n";
        return -1;
    }

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/finaldraft.otf")) {
        cout << "Failed to load font!" << endl;
        return -1;
    }

    //  text manager
    GameTextManager textManager(font);

    Player player("assets/Player/Player.png", 1.0f, 200.f);
    player.setPosition({ TILE_SIZE * 2.f, TILE_SIZE * 2.f });
    player.setRow(3);

    std::vector<AnimatedEnemy> enemies;
    AnimatedEnemy enemy("assets/zombie/Enemy.png", "assets/Sounds/collison_sound.wav", "assets/Sounds/followingsound.wav");
    enemy.setRow(0);
    enemy.setPosition(100.f, 150.f);
    enemies.push_back(enemy);

    AnimatedEnemy enemy2("assets/zombie/Enemy.png", "assets/Sounds/collison_sound.wav", "assets/Sounds/followingsound.wav");
    enemy2.setRow(2);
    enemy2.setPosition(500.f, 100.f);
    enemies.push_back(enemy2);

    AnimatedEnemy enemy3("assets/zombie/Enemy.png", "assets/Sounds/collison_sound.wav", "assets/Sounds/followingsound.wav");
    enemy3.setRow(1);
    enemy3.setPosition(320.f, 450.f);
    enemies.push_back(enemy3);

    // Loading the sound once at the start of the game
    Coin::loadSound("assets/Sounds/coin_pickup.wav");
    std::vector<Coin> coins;
    coins.emplace_back(coinTexture, 5 * TILE_SIZE, 5 * TILE_SIZE);
    coins.emplace_back(coinTexture, 16 * TILE_SIZE, 1 * TILE_SIZE);
    coins.emplace_back(coinTexture, 9 * TILE_SIZE, 3 * TILE_SIZE);
    coins.emplace_back(coinTexture, 4* TILE_SIZE, 11 * TILE_SIZE);
    coins.emplace_back(coinTexture, 19* TILE_SIZE, 14* TILE_SIZE);
    int score = 0;
    sf::Clock clock;
    sf::Clock damageCooldown;

    int currentLevel = 1;
    int (*currentGrid)[GRID_WIDTH] = gridLevel1;

    sf::RectangleShape healthBarBackground(sf::Vector2f(150.f, 20.f));
    sf::RectangleShape healthBarForeground(sf::Vector2f(150.f, 20.f));
    healthBarBackground.setFillColor(sf::Color(75, 75, 75));
    healthBarForeground.setFillColor(sf::Color::Green);
    healthBarBackground.setPosition(1.f, 570.f);
    healthBarForeground.setPosition(1.f, 570.f);

    MainMenu menu(window, font);
    bool gameRunning = false;
    sf::Music menumusic;
    sf::Music gamemusic;
    bool gamemusicstart = false;

    if (!menumusic.openFromFile("assets/Sounds/menumusic.ogg")) {
        std::cout << "Failed to load menu music!" << std::endl;
    }
    else {
        menumusic.setLoop(true);
        menumusic.play();
    }

    if (!gamemusic.openFromFile("assets/Sounds/gamemusic.ogg")) {
        std::cout << "Failed to load game music!" << std::endl;
    }
    //---------------------End of texture loading and position placing--------------------------------------


    //------------------------------------MAIN GAME LOOP WITH LOGICS ---------------------------------------------------------
    while (window.isOpen())
    {
        if (!gameRunning) {
            menu.handleInput(gameRunning);
            menu.draw();
        }

        if (gameRunning) {
            if (!gamemusicstart) {
                menumusic.stop();
                sf::sleep(sf::milliseconds(500));
                gamemusic.setLoop(true);
                gamemusic.play();
                gamemusic.setVolume(30);
                gamemusicstart = true;
            }

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            //------   MOVING TO NEXT LEVEL WITH N  -----------//
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                if (currentLevel == 1 && coins.empty()) {
                    currentLevel = 2;
                    currentGrid = gridLevel2; // or gridLevel3

                    enemies[0].setRow(3);
                    enemies[1].setRow(2);
                    enemies[2].setRow(1);


                    player.setPosition(sf::Vector2f(100, 50));
                    coins.clear();
                    coins.emplace_back(coinTexture, 2 * TILE_SIZE, 7 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 18 * TILE_SIZE, 8 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 5* TILE_SIZE, 13 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 18 * TILE_SIZE, 11 * TILE_SIZE);

                    decorations.clear();
                    decorations.emplace_back(treeTexture, 5 * TILE_SIZE, 4 * TILE_SIZE, true, DecorationType::Tree);
                    decorations.emplace_back(treeTexture, 14* TILE_SIZE, 6 * TILE_SIZE, true, DecorationType::Tree);
                    decorations.emplace_back(treeTexture, 2 * TILE_SIZE, 9 * TILE_SIZE, true, DecorationType::Tree);
                    decorations.emplace_back(treeTexture, 22* TILE_SIZE, 13.5* TILE_SIZE, true, DecorationType::Tree);
                    decorations.emplace_back(fencetexture, 1 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 2.3* TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 3.9 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 4.8 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 5.8 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 14 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 15.3* TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 17.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 18.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 20. * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                    decorations.emplace_back(fencetexture, 21.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                }
                else if (currentLevel == 2 && coins.empty()) {
                    currentLevel = 3;
                    currentGrid = gridLevel3;
                    player.setPosition(sf::Vector2f(50,50));
                    decorations.clear();
                    coins.clear();
                    coins.emplace_back(coinTexture, 8 * TILE_SIZE, 8 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 11 * TILE_SIZE, 6 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 16 * TILE_SIZE, 3 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 12 * TILE_SIZE, 9 * TILE_SIZE);
                    coins.emplace_back(coinTexture, 18 * TILE_SIZE, 10 * TILE_SIZE);

                    enemies[0].setRow(1);
                    enemies[0].setPosition(400, 400);

                    for (auto& enemy : enemies) {
                        enemy.setScale(2.0f, 2.0f);
                    }

                    if (enemies.size() > 1) {
                        enemies.erase(enemies.begin() + 1);
                    }
                }
            }


            // Update displayed level and score
            textManager.updateLevel(currentLevel);
            textManager.updateScore(score);

            //SAVING THE STATE OF GAME  AND LOADING//
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                std::ofstream saveFile("savegame.txt");
                if (saveFile.is_open()) {
                    saveFile << player.getPosition().x << " " << player.getPosition().y << "\n";
                    saveFile << playerhealth << " " << score << "\n";
                    saveFile << currentLevel << "\n";
                    saveFile << coins.size() << "\n";
                    for (const auto& coin : coins) {
                        sf::Vector2f pos = coin.getPosition();
                        saveFile << pos.x << " " << pos.y << "\n";
                    }
                    std::cout << "Game saved!\n";
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                std::ifstream loadFile("savegame.txt");
                if (loadFile.is_open()) {
                    float x, y;
                    loadFile >> x >> y;
                    player.setPosition({ x, y });
                    loadFile >> playerhealth >> score;
                    loadFile >> currentLevel;



                    // For drawing the grid based on each level
                    if (currentLevel == 1) {
                        currentGrid = gridLevel1;
                    }
                    else if (currentLevel == 2) {
                        currentGrid = gridLevel2;
                    }
                    else if (currentLevel == 3) {
                        currentGrid = gridLevel3;
                    }
                    // Clear existing coins and decorations
                    coins.clear();
                    decorations.clear();

                    // Load coins
                    size_t coinCount;
                    loadFile >> coinCount;
                    for (size_t i = 0; i < coinCount; ++i) {
                        loadFile >> x >> y;
                        coins.emplace_back(coinTexture, x, y);
                    }
                    // Reinitialize decorations based on the loaded level
                    if (currentLevel == 1) {
                        // Decorations for Level 1
                        decorations.emplace_back(treeTexture, 1 * TILE_SIZE, 9 * TILE_SIZE, true, DecorationType::Tree);
                        decorations.emplace_back(shadowTexture, 19 * TILE_SIZE, 13 * TILE_SIZE, true, DecorationType::Shadow);
                        decorations.emplace_back(treeTexture, 20 * TILE_SIZE, 12 * TILE_SIZE, true, DecorationType::Tree);
                 
                    }
                    else if (currentLevel == 2) {
                        // Decorations for Level 2
                        decorations.emplace_back(treeTexture, 5 * TILE_SIZE, 4 * TILE_SIZE, true, DecorationType::Tree);
                        decorations.emplace_back(treeTexture, 14 * TILE_SIZE, 6 * TILE_SIZE, true, DecorationType::Tree);
                        decorations.emplace_back(treeTexture, 2 * TILE_SIZE, 9 * TILE_SIZE, true, DecorationType::Tree);
                        decorations.emplace_back(treeTexture, 22 * TILE_SIZE, 13.5 * TILE_SIZE, true, DecorationType::Tree);
                        decorations.emplace_back(fencetexture, 1 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 2.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 3.9 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 4.8 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 5.8 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 14 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 15.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 17.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 18.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 20. * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);
                        decorations.emplace_back(fencetexture, 21.3 * TILE_SIZE, 15.4 * TILE_SIZE, true, DecorationType::Fence);

                    }
                    else if (currentLevel == 3) {
                 
                    }

                    std::cout << "Game loaded!\n";
                }
            }            //----------------------------------END OF SAVING AND LOADING GAME-------------------------------------------------------

            //------Updating enemies and player begin------------
            float deltaTime = clock.restart().asSeconds();
            player.storePosition();
            player.handleInput(deltaTime);
            
            player.update(deltaTime);

            for (auto& enemy : enemies) {
                enemy.update(deltaTime);
                if (currentLevel == 3) {
                    sf::Vector2f playerPos = player.getPosition();
                    enemy.move(deltaTime, playerPos);
                    sf::Vector2f direction = player.getPosition() - enemy.getPosition();
                    if (std::abs(direction.x) > std::abs(direction.y)) {
                        enemy.setRow(direction.x > 0 ? 3 : 2);
                    }
                    else {
                        enemy.setRow(direction.y > 0 ? 0 : 1);
                    }
                }
            }
//---------updating enemies and player end-------------------------

    //-------------------wall collision detection start-----------//
            sf::FloatRect bounds = player.getBounds();
            auto getTileCoords = [&](float x, float y) {
                return sf::Vector2i(
                    static_cast<int>(x / TILE_SIZE),
                    static_cast<int>(y / TILE_SIZE)  );
                };
            sf::Vector2i topLeft = getTileCoords(bounds.left, bounds.top);
            sf::Vector2i topRight = getTileCoords(bounds.left + bounds.width, bounds.top);
            sf::Vector2i bottomLeft = getTileCoords(bounds.left, bounds.top + bounds.height);
            sf::Vector2i bottomRight = getTileCoords(bounds.left + bounds.width, bounds.top + bounds.height);

            auto isWall = [&](int x, int y) -> bool {
                if (x < 0 || y < 0 || x >= GRID_WIDTH || y >= GRID_HEIGHT)
                    return true;
                return currentGrid[y][x] == 1; // Now uses the current level's grid
                };

            if (isWall(topLeft.x, topLeft.y) || isWall(topRight.x, topRight.y) ||
                isWall(bottomLeft.x, bottomLeft.y) || isWall(bottomRight.x, bottomRight.y)) {
                player.revertToLastPosition();
            }
            //-------------------wall collision detection End-----------//


            //-------------Decoration collision logic------------//
            for (auto& decoration : decorations) {
                if (decoration.getType() == DecorationType::Tree && decoration.isBlocking()) {
                    if (player.getBounds().intersects(decoration.getBounds())) {
                        decoration.playCollisionSound();
                        player.revertToLastPosition();
                    }
                }
            }           
            //-------------Decoration collision logic end------------//


            //--------------Enemies collision start---------------//
            for (auto& enemy : enemies) {
                if (player.getBounds().intersects(enemy.getBounds())) {
                    if (damageCooldown.getElapsedTime().asSeconds() > 1.0f) {
                        playerhealth -= 10;
                        player.revertToLastPosition();
                        enemy.playCollisionSound();
                        damageCooldown.restart();
                        std::cout << "Player hit! Health: " << playerhealth << std::endl;
                        if (playerhealth <= 0) {
                            textManager.playGameOverSound();

                            sf::Time gameOverDuration = textManager.getGameOverSoundDuration();
                            sf::Clock soundClock;
                            gameRunning = false;
                            gamemusic.stop();

                            // Waiting until the game over sound is finished
                            while (soundClock.getElapsedTime() < gameOverDuration) {
                                // Allowing the sound to finish
                                window.clear(sf::Color::Black);

                                window.draw(background);
                                textManager.drawGameOverText(window);

                                window.display();
                            }
                            window.close();                                        
                        }
                    }
                }
            }
            //------------ COIN COLLECTION LOGIC----------------------//    
            for (auto it = coins.begin(); it != coins.end();) {
                if (it->getBounds().intersects(player.getBounds()) && !it->isCollected()) {
                    it->collect();
                    score += 10;
                    it = coins.erase(it);
                }
                else {
                    ++it;
                }
            }//--------------Game completion logic----------------------------------//
            if (currentLevel == 3 && coins.empty()) {
                gameRunning = false;
                gamemusic.stop();
                std::cout << "Game Completed!" << std::endl;
                window.clear(sf::Color::Black);
                window.draw(background);
                textManager.drawCompletionText(window); //  Drawing "Game Completed!"
                textManager.playCompletionSound();      // sound effect on completion
                window.display();
                sf::sleep(sf::seconds(4));

                window.close();
            }
            float healthPercentage = playerhealth / maxhealth;
            healthBarForeground.setSize(sf::Vector2f(150.f * healthPercentage, 20.f));

//----------------------------------DRAWING SECTION OF THE GAME ------------------------------------------  //
            window.clear(sf::Color::Black);
            if (currentLevel == 3)
                window.draw(level3bg);  // Use the special background for level 3
            else
                window.draw(background);  // Use default for levels 1 and 2
          
            if (currentLevel == 1)
                drawGrid(gridLevel1, window, wallTexture);
            else if (currentLevel == 2)
                drawGrid(gridLevel2, window, wallTexture);
            else if (currentLevel == 3)
                drawGrid(gridLevel3, window, wallTexture);
            for (auto& obj : decorations) {
                obj.draw(window);
            }


       
            for (auto& enemy : enemies) enemy.draw(window);
            for (auto& coin : coins) coin.draw(window);


            player.draw(window);
          

            textManager.setNextLevelVisible(coins.empty());

            textManager.draw(window); // ✅ Draw score, level, etc.


            window.draw(healthBarBackground);
            window.draw(healthBarForeground);
            window.display();
        }
    }


    return 0;
}
