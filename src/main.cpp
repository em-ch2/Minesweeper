#include <iostream>
#include <fstream>
#include <cstdlib>
#include "WelcomeWindow.h"
#include "Board.h"
#include "Tiles.h"

using namespace std;

struct Config {
    int col;
    int row;
    int mines;
};

Config loadConfig() {
    ifstream config_file("files/config.cfg");
    Config config;

    if (!config_file.is_open()) {
        cout << "Error. Config File cannot open.";
        config.col = 25;
        config.row = 16;
        config.mines = 50;
        return config;
    }

    config_file >> config.col >> config.row >> config.mines;
    return config;
}

int main() {
    srand(time(0));
    Config dimensions = loadConfig();

    int width = dimensions.col * 32;
    int height = (dimensions.row * 32) + 100;

    WelcomeWindow welcome(width, height);
    welcome.displayWindow();

    if (!welcome.gameOpen) {
        return 0;
    }

    string playerName = welcome.getUserName();
    if (playerName.empty()) {
        return 0;
    }

    Tiles game(dimensions.col, dimensions.row, width, height, dimensions.mines, playerName);
    game.displayWindow();

    return 0;
}