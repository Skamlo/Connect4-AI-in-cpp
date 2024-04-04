#pragma once

#include "Settings.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#define ROW_COUNT 6
#define COLUMN_COUNT 7

void resetBoard(int (*board)[COLUMN_COUNT]);
bool isValidColumn(int(*board)[COLUMN_COUNT], int column);
int* getValidLocations(int(*board)[COLUMN_COUNT]);
void dropPiece(int(*board)[COLUMN_COUNT], int column, int piece);
int winningWindow(int window[4], int piece);
bool winningMove(int(*board)[COLUMN_COUNT], int piece);
bool isDraw(int(*board)[COLUMN_COUNT]);

void drawBoard(
    sf::RenderWindow* window,
    int(*board)[COLUMN_COUNT],
    int xPos, int yPos,
    int squareSize,
    sf::Vector2f mousePosition,
    Colors colors
);

int renderBoard(
	sf::RenderWindow* window,
    int (*board)[COLUMN_COUNT],
    sf::Font font,
    sf::Vector2f mousePosition,
    bool isMouseClicked,
    bool isMouseHeld,
    int* currentPlayer,
    Colors colors,
    int* difficultyLevel,
    int currentPage,
    int* state,
    int* waitTimer
);
