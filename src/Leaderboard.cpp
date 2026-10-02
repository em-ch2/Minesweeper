#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "fstream"
#include "sstream"
#include <algorithm>
#include "Leaderboard.h"

#include <algorithm>
using namespace std;

Leaderboard::Leaderboard(int co, int ro, string na, int ti, bool won) {
    col = co;
    row = ro;
    userName = na;
    userTime = ti;
    gameWon = won;
}

void Leaderboard::displayWindow() {
    int width = col * 16;
    int height = row * 16 + 50;

    window.create(
    sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}),
    "Minesweeper",
    sf::Style::Titlebar | sf::Style::Close
    );

    if (!font.openFromFile("files/font.ttf")) {
        std::cout << "Error loading font";
    }

    vector<Entries> fileEntries = readEntries();
    bool newScore = false;

    if (gameWon) {
        Entries e;
        e.time = userTime;
        e.name = userName;
        fileEntries.push_back(e);

        sort(fileEntries.begin(), fileEntries.end(), [](Entries a, Entries b){return a.time < b.time;});

        if (fileEntries.size() > 5) {
            fileEntries.resize(5);
        }

        for (int i = 0; i < (int)fileEntries.size(); i++) {
            if (fileEntries[i].time == userTime && fileEntries[i].name == userName) {
                newScore = true;
            }
        }

        if (newScore) {
            writeEntries(fileEntries);
        }
    }
    else {
        sort(fileEntries.begin(), fileEntries.end(), [](Entries a, Entries b){return a.time < b.time;});

        if (fileEntries.size() > 5) {
            fileEntries.resize(5);
        }
    }

    int rank = -1;
    if (newScore) {
        for (int i = 0; i < (int)fileEntries.size(); i++) {
            if (fileEntries[i].time == userTime && fileEntries[i].name == userName) {
                rank = i;
                break;
            }
        }
    }

    string leaderboardString;
    for (int i = 0; i < (int)fileEntries.size(); i++) {
        int entryTime = fileEntries[i].time;
        int minutes = entryTime / 60;
        int seconds = entryTime % 60;

        string minuteString = (minutes < 10 ? "0" : "") + to_string(minutes);
        string secondString = (seconds < 10 ? "0" : "") + to_string(seconds);

        leaderboardString += to_string(i + 1) + ".\t" +
                             minuteString + ":" + secondString + "\t" +
                             fileEntries[i].name;

        if (i == rank) {
            leaderboardString += "*";
        }

        if (i != static_cast<int>(fileEntries.size()) - 1) {
            leaderboardString += "\n\n";
        }
    }

    sf::Text leaderboardTitle(font, "LEADERBOARD", 20);
    leaderboardTitle.setFillColor(sf::Color::White);
    leaderboardTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardTitle, width / 2.0f, height / 2.0f - 120.0f);

    sf::Text leaderboardContent(font, leaderboardString, 18);
    leaderboardContent.setFillColor(sf::Color::White);
    leaderboardContent.setStyle(sf::Text::Bold);
    setText(leaderboardContent, width / 2.0f, height / 2.0f + 20.0f);

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(leaderboardTitle);
        window.draw(leaderboardContent);
        window.display();
    }
}

vector<Entries> Leaderboard::readEntries(){
    vector<Entries> entries;
    ifstream file("files/leaderboard.txt");

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return vector<Entries>();
    }

    string line;
    while (getline(file, line)) {
        Entries e;
        stringstream ss(line);
        string stringTime;
        string fileNames;

        getline(ss, stringTime, ',');
        getline(ss, fileNames);

        fileNames.erase(0, std::min(fileNames.find_first_not_of(" "), fileNames.size()));

        int minutes = stoi(stringTime.substr(0, 2));
        int seconds = stoi(stringTime.substr(3, 2));
        e.time = minutes * 60 + seconds;
        e.name = fileNames;

        entries.push_back(e);
    }

    return entries;
}

void Leaderboard:: writeEntries(vector<Entries>& entries) {
    ofstream file("files/leaderboard.txt");

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    if (entries.size() > 5) {
        entries.resize(5);
    }

    for (int i = 0; i < (int)entries.size(); i++) {
        int entryTime = entries[i].time;

        int minutes = entryTime / 60;
        int seconds = entryTime % 60;

        if (minutes < 10) {
            file << "0";
        }
        file << minutes << ":";

        if (seconds < 10) {
            file << "0";
        }
        file << seconds << "," << entries[i].name << endl;
    }
}

void Leaderboard::setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f,
                                textRect.position.y + textRect.size.y / 2.0f));
    text.setPosition(sf::Vector2f(x, y));
}