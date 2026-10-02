#include "WelcomeWindow.h"
#include <cctype>
#include <optional>
#include <stdexcept>
#include <iostream>

using namespace std;

WelcomeWindow::WelcomeWindow(int w, int h) :
    title(font, "", 24),
    prompt(font, "", 20),
    nameText(font, "", 18){
    width = w;
    height = h;
    userName = "";

    window.create(
        sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}),
        "Minesweeper",
        sf::Style::Titlebar | sf::Style::Close
    );

    if (!font.openFromFile("files/font.ttf")) {
        std::cout << "Error loading font";
    }

    title = sf::Text(font, "WELCOME TO MINESWEEPER!", 24);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(title, width / 2.0f, height / 2.0f - 150.0f);

    prompt = sf::Text(font, "Enter your name:", 20);
    prompt.setFillColor(sf::Color::White);
    prompt.setStyle(sf::Text::Bold);
    setText(prompt, width / 2.0f, height / 2.0f - 75.0f);

    nameText = sf::Text(font, "|", 18);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, width / 2.0f, height / 2.0f - 45.0f);
}

std::string WelcomeWindow::getUserName() const {
    return userName;
}

void WelcomeWindow::displayWindow() {
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                gameOpen = false;
                window.close();
            }

            else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                unsigned int unicode = textEntered->unicode;

                if (isalpha(unicode) && userName.size() < 10) {
                    nameFormat(unicode);
                    nameText.setString(userName + "|");
                    setText(nameText, width / 2.0f, height / 2.0f - 45.0f);
                }
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Backspace && !userName.empty()) {
                    userName.pop_back();
                    nameText.setString(userName + "|");
                    setText(nameText, width / 2.0f, height / 2.0f - 45.0f);
                }

                if (keyPressed->code == sf::Keyboard::Key::Enter && !userName.empty()) {
                    gameOpen = true;
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(prompt);
        window.draw(nameText);
        window.display();
    }
};

void WelcomeWindow::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f,
                                textRect.position.y + textRect.size.y / 2.0f));
    text.setPosition(sf::Vector2f(x, y));
}

void WelcomeWindow::nameFormat(unsigned int unicode) {
    if (userName.empty()) {
        userName += toupper(static_cast<char>(unicode));
    }
    else {
        userName += tolower(static_cast<char>(unicode));
    }
}