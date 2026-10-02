#ifndef CHEN_EMILY_PROJECT3_SPRING2026_WELCOME_WINDOW_H
#define CHEN_EMILY_PROJECT3_SPRING2026_WELCOME_WINDOW_H

#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class WelcomeWindow {
private:
    int width, height;

    sf::RenderWindow window;
    sf::Font font;
    sf::Text title;
    sf::Text prompt;
    sf::Text nameText;

    std::string userName;

public:
    bool gameOpen = false;
    WelcomeWindow(int w, int h);
    std::string getUserName() const;
    void displayWindow();
    void setText(sf::Text &text, float x, float y);
    void nameFormat(unsigned int unicode);
};

#endif