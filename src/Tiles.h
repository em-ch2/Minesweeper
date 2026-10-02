#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include <string>
#include <chrono>

#ifndef CHEN_EMILY_PROJECT3_SPRING2026_GAMEWINDOW_H
#define CHEN_EMILY_PROJECT3_SPRING2026_GAMEWINDOW_H

class Tiles {
private:
    int col, row, width, height, mines;
    int timer = 0;
    bool paused = false;
    bool debug = false;
    bool leaderboardOpen = false;
    std::string userName;

    std::chrono::steady_clock::time_point startTime;

    sf::RenderWindow window;
    sf::Font font;
    sf::Texture tileHidden;
    sf::Texture tileRevealed;
    sf::Texture mine;
    sf::Texture flag;
    sf::Texture happyFace;
    sf::Texture winFace;
    sf::Texture loseFace;
    sf::Texture debugButton;
    sf::Texture pauseButton;
    sf::Texture playButton;
    sf::Texture leaderboardButton;
    sf::Texture digits;
    sf::Texture numbers[8];

    Board board;

public:
    Tiles(int co, int ro, int wi, int he, int mi, std::string na);
    void displayWindow();
    void drawBoard();
    void drawButtons();
    void drawDigit(int digit, float x, float y);
    void drawMineCounter();
    void drawTimer();
    int calculateTimer();
};

#endif //CHEN_EMILY_PROJECT3_SPRING2026_GAMEWINDOW_H