#ifndef CHEN_EMILY_PROJECT3_SPRING2026_LEADERBOARD_H
#define CHEN_EMILY_PROJECT3_SPRING2026_LEADERBOARD_H

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

struct Entries {
    std::string name;
    int time;
};

class Leaderboard {
private:
    int col, row;
    std::string userName;
    int userTime;
    bool gameWon;

    sf::RenderWindow window;
    sf::Font font;

public:
    Leaderboard(int co, int ro, std::string na, int ti, bool won);;
    void displayWindow();
    std::vector<Entries> readEntries();
    void writeEntries(std::vector<Entries>& entries);
    void setText(sf::Text &text, float x, float y);
};

#endif //CHEN_EMILY_PROJECT3_SPRING2026_LEADERBOARD_H