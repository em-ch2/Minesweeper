#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include "Board.h"
#include <cstdlib>

using namespace std;

Board::Board(int co, int ro, int mi) {
    col = co;
    row = ro;
    mines = mi;
    flagCount = 0;

    grid.resize(row, vector<Tile>(col));
}

void Board::flagTile(int co, int ro) {
    if (gameWon || gameLost) {
        return;
    }

    Tile& tile = grid[ro][co];

    if (tile.revealed) {
        return;
    }

    if (tile.hasFlag) {
        tile.hasFlag = false;
        flagCount--;
    }
    else {
        tile.hasFlag = true;
        flagCount++;
    }
}

void Board::revealTile(int co, int ro) {
    if (co < 0 || ro < 0 || co >= col || ro >= row) {
        return;
    }

    Tile& tile = grid[ro][co];

    if (tile.hasFlag) {
        return;
    }

    if (tile.revealed) {
        return;
    }

    if (tile.hasMine) {
        gameLost = true;
        tile.revealed = true;
        return;
    }

    tile.revealed = true;
    if (tile.adjacentMines == 0) {
        for (int adjC = -1; adjC <= 1; adjC++) {
            for (int adjR = -1; adjR <= 1; adjR++) {
                if (adjC != 0 || adjR != 0) {
                    revealTile(co + adjC, ro + adjR);
                }
            }
        }
    }

    if (checkVictory()) {
        gameWon = true;
    }
}

Tile& Board::getTile(int col, int row) {
    return grid[row][col];
}

void Board::generateMines() {
    int mineTiles = 0;
    while (mineTiles < mines) {
        int randomCol = rand() % col;
        int randomRow = rand() % row;

        if (!grid[randomRow][randomCol].hasMine) {
            grid[randomRow][randomCol].hasMine = true;
            mineTiles++;
        }
    }

    for (int ro = 0; ro < row; ro++) {
        for (int co = 0; co < col; co++) {
            int count = 0;

            for (int adjC = -1; adjC <= 1; adjC++) {
                for (int adjR = -1; adjR <= 1; adjR++) {
                    if (adjC == 0 && adjR == 0) {
                        continue;
                    }

                    int neighborCol = co + adjC;
                    int neighborRow = ro + adjR;

                    if (neighborCol >= 0 && neighborRow >= 0 && neighborCol < col && neighborRow < row) {
                        if (grid[neighborRow][neighborCol].hasMine) {
                            count++;
                        }
                    }
                }
            }
            grid[ro][co].adjacentMines = count;
        }
    }
}

bool Board::checkVictory() {
    for (int ro = 0; ro < row; ro++) {
        for (int co = 0; co < col; co++) {
            if (!grid[ro][co].hasMine && !grid[ro][co].revealed) {
                return false;
            }
        }
    }
    return true;
}

bool Board::getGameWon() const{
    return gameWon;
}

bool Board::getGameLost() const {
    return gameLost;
}

int Board::getMineCount() const {
    return mines - flagCount;
}

void Board::resetBoard() {
    flagCount = 0;
    gameWon = false;
    gameLost = false;

    for (int ro = 0; ro < row; ro++) {
        for (int co = 0; co < col; co++) {
            grid[ro][co] = Tile();
        }
    }

    generateMines();
}

void Board::flagRemainingMines() {
    for (int ro = 0; ro < row; ro++) {
        for (int co = 0; co < col; co++) {
            if (grid[ro][co].hasMine && !grid[ro][co].hasFlag) {
                grid[ro][co].hasFlag = true;
                flagCount++;
            }
        }
    }
}

void Board::revealAllMines() {
    for (int ro = 0; ro < row; ro++)
        for (int co = 0; co < col; co++)
            if (grid[ro][co].hasMine)
                grid[ro][co].revealed = true;
}