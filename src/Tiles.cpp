#include "Tiles.h"
#include "Board.h"
#include "Leaderboard.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

Tiles::Tiles(int co, int ro, int wi, int he, int mi, std::string na) : board(co, ro, mi) {
    col = co;
    row = ro;
    width = wi;
    height = he;
    mines = mi;
    userName = na;

    startTime = std::chrono::steady_clock::now();

    window.create(
    sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}),
    "Minesweeper",
    sf::Style::Titlebar | sf::Style::Close
    );

    if (!font.openFromFile("files/font.ttf")) {
        std::cout << "Error loading font";
    }

    tileHidden.loadFromFile("files/images/tile_hidden.png");
    tileRevealed.loadFromFile("files/images/tile_revealed.png");
    mine.loadFromFile("files/images/mine.png");
    flag.loadFromFile("files/images/flag.png");
    debugButton.loadFromFile("files/images/debug.png");
    pauseButton.loadFromFile("files/images/pause.png");
    playButton.loadFromFile("files/images/play.png");
    leaderboardButton.loadFromFile("files/images/leaderboard.png");
    digits.loadFromFile("files/images/digits.png");
    happyFace.loadFromFile("files/images/face_happy.png");
    loseFace.loadFromFile("files/images/face_lose.png");
    winFace.loadFromFile("files/images/face_win.png");

    for (int i = 0; i < 8; i++) {
        numbers[i].loadFromFile("files/images/number_" + to_string(i+1) + ".png");
    }

    board.generateMines();
}

void Tiles::displayWindow() {
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                int mouseX = mouseReleased->position.x;
                int mouseY = mouseReleased->position.y;
                int tileCol = mouseX / 32;
                int tileRow = mouseY / 32;

                float buttonY = 32 * (row + 0.5f);
                float happyX = (col / 2.0f) * 32 - 32;
                float debugX = col * 32 - 304;
                float pauseX = col * 32 - 240;
                float leaderboardX = col * 32 - 176;

                if (mouseReleased->button == sf::Mouse::Button::Left) {
                    // Face button always works
                    if (mouseX >= happyX && mouseX <= happyX + 64 &&
                        mouseY >= buttonY && mouseY <= buttonY + 64) {
                        board.resetBoard();
                        timer = 0;
                        startTime = std::chrono::steady_clock::now();
                        paused = false;
                        debug = false;
                    }

                    // Leaderboard button always works
                    else if (mouseX >= leaderboardX && mouseX <= leaderboardX + 64 &&
                             mouseY >= buttonY && mouseY <= buttonY + 64) {
                        bool wasPaused = paused;
                        leaderboardOpen = true;

                        Leaderboard leaderboard(col, row, userName, timer, board.getGameWon());
                        leaderboard.displayWindow();

                        leaderboardOpen = false;
                        paused = wasPaused;
                    }

                    // Play/Pause button works only if game is not won/lost
                    else if (mouseX >= pauseX && mouseX <= pauseX + 64 &&
                             mouseY >= buttonY && mouseY <= buttonY + 64 &&
                             !board.getGameWon() && !board.getGameLost()) {
                        if (!paused) {
                            paused = true;
                        }
                        else {
                            paused = false;
                            startTime = std::chrono::steady_clock::now() - std::chrono::seconds(timer);
                        }
                    }

                    // Debug button only when not paused and not game over
                    else if (!paused && !board.getGameWon() && !board.getGameLost() &&
                             mouseX >= debugX && mouseX <= debugX + 64 &&
                             mouseY >= buttonY && mouseY <= buttonY + 64) {
                        debug = !debug;
                    }

                    // Tile reveal only when not paused and not game over
                    else if (!paused && !board.getGameWon() && !board.getGameLost() &&
                             tileRow < row) {
                        board.revealTile(tileCol, tileRow);

                        if (board.getGameWon()) {
                            board.flagRemainingMines();
                        }
                        if (board.getGameLost()) {
                            board.revealAllMines();
                        }
                    }
                }

                else if (mouseReleased->button == sf::Mouse::Button::Right) {
                    if (!paused && !board.getGameWon() && !board.getGameLost() && tileRow < row) {
                        board.flagTile(tileCol, tileRow);
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        calculateTimer();
        drawBoard();
        drawButtons();
        drawMineCounter();
        drawTimer();
        window.display();
    }
}

void Tiles::drawBoard() {
    for (int ro = 0; ro < row; ro++) {
        for (int co = 0; co < col; co++) {
            Tile& tile = board.getTile(co, ro);

            if (paused || leaderboardOpen) {
                sf::Sprite revealSprite(tileRevealed);
                revealSprite.setPosition({co * 32.0f, ro * 32.0f});
                window.draw(revealSprite);
                continue;
            }

            if (tile.revealed) {
                sf::Sprite revealSprite(tileRevealed);
                revealSprite.setPosition({co * 32.0f, ro * 32.0f});
                window.draw(revealSprite);

                if (tile.hasMine) {
                    sf::Sprite mineSprite(mine);
                    mineSprite.setPosition({co * 32.0f, ro * 32.0f});
                    window.draw(mineSprite);
                }
                else if (tile.adjacentMines > 0) {
                    sf::Sprite numberSprite(numbers[tile.adjacentMines - 1]);
                    numberSprite.setPosition({co * 32.0f, ro * 32.0f});
                    window.draw(numberSprite);
                }
            }

            else if (tile.hasFlag) {
                sf::Sprite tileBackground(tileHidden);
                tileBackground.setPosition({co * 32.0f, ro * 32.0f});
                window.draw(tileBackground);

                sf::Sprite flagSprite(flag);
                flagSprite.setPosition({co * 32.0f, ro * 32.0f});
                window.draw(flagSprite);
            }

            else {
                sf::Sprite tileSprite(tileHidden);
                tileSprite.setPosition({co * 32.0f, ro * 32.0f});
                window.draw(tileSprite);

                if (debug && tile.hasMine) {
                    sf::Sprite mineSprite(mine);
                    mineSprite.setPosition({co * 32.0f, ro * 32.0f});
                    window.draw(mineSprite);
                }
            }
        }
    }
}

void Tiles::drawButtons() {
    float happyX = (col / 2.0f) * 32 - 32;
    float y = 32 * (row + 0.5f);
    sf::Texture* face;

    if (board.getGameWon()) {
        face = &winFace;
    }

    else if (board.getGameLost()) {
        face = &loseFace;
    }

    else {
        face = &happyFace;
    }

    sf::Sprite faceSprite(*face);
    faceSprite.setPosition({happyX, y});
    window.draw(faceSprite);

    float debugX = (col * 32 - 304);

    sf::Sprite debugSprite(debugButton);
    debugSprite.setPosition({debugX, y});
    window.draw(debugSprite);

    float pauseX = (col * 32 - 240);

    if (paused) {
        sf::Sprite playSprite (playButton);
        playSprite.setPosition({pauseX, y});
        window.draw(playSprite);
    }
    else {
        sf::Sprite pauseSprite (pauseButton);
        pauseSprite.setPosition({pauseX, y});
        window.draw(pauseSprite);
    }

    float leaderboardX = (col * 32 - 176);

    sf::Sprite leaderboardSprite (leaderboardButton);
    leaderboardSprite.setPosition({leaderboardX, y});
    window.draw(leaderboardSprite);
}

void Tiles::drawDigit(int digit, float x, float y) {
    sf::Sprite digitSprite(digits);
    digitSprite.setTextureRect(sf::IntRect({digit * 21, 0}, {21, 32}));
    digitSprite.setPosition({x, y});
    window.draw(digitSprite);
};

void Tiles::drawMineCounter() {
    int mineCount = board.getMineCount();
    float mineX = 33;
    float mineY = 32 * (row + 0.5f) + 16;

    if (mineCount < 0) {
        drawDigit(10, 12, mineY);

        int absMineCount = abs(mineCount);
        drawDigit(absMineCount / 100, mineX, mineY);
        drawDigit(absMineCount / 10 % 10, mineX + 21, mineY);
        drawDigit(absMineCount % 10, mineX + 42, mineY);

        return;
    }
    drawDigit(mineCount / 100, mineX, mineY);
    drawDigit(mineCount / 10 % 10, mineX + 21, mineY);
    drawDigit(mineCount  % 10, mineX + 42, mineY);
};

void Tiles::drawTimer() {
    int minutes = timer / 60;
    int seconds = timer % 60;

    float timerY = 32 * (row + 0.5f) + 16;
    float minutesX = (col * 32 - 97);
    float secondsX = (col * 32 - 54);

    drawDigit(minutes / 10, minutesX, timerY);
    drawDigit(minutes % 10, minutesX + 21, timerY);

    drawDigit(seconds / 10, secondsX, timerY);
    drawDigit(seconds % 10, secondsX + 21, timerY);
};

int Tiles::calculateTimer() {
    if (!paused && !board.getGameWon() && !board.getGameLost()) {
        auto now = std::chrono::steady_clock::now();
        timer = std::chrono::duration_cast<std::chrono::seconds>(now-startTime).count();
    }

    return timer;
}
