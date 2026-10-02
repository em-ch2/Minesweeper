#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#pragma once

struct Tile {
    bool hasMine = false;
    bool hasFlag = false;
    bool revealed = false;
    int adjacentMines = 0;
};

class Board {
private:
    sf::RenderWindow window;
    int col, row, mines;
    std::vector<std::vector<Tile>> grid;
    bool gameWon = false;
    bool gameLost = false;
    int flagCount;

public:
    Board(int co, int ro, int mi);
    void flagTile(int col, int row);
    void revealTile(int col, int row);
    Tile& getTile(int col, int row);
    void generateMines();
    bool checkVictory();
    bool getGameWon() const;
    bool getGameLost() const;
    int getMineCount() const;
    void resetBoard();
    void flagRemainingMines();
    void revealAllMines();
};