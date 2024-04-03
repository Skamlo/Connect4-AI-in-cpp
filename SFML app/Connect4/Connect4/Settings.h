#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

struct Colors
{
    sf::Color backgroundColor;
    sf::Color hoverBackgroundColor;
    sf::Color foregroundColor;
    sf::Color hoverForegroundColor;
    sf::Color firstPlayerColor;
    sf::Color hoverFirstPlayerColor;
    sf::Color secondPlayerColor;
    sf::Color hoverSecondPlayerColor;
    sf::Color white;
    sf::Color dirtyWhite;
    sf::Color empty;
    sf::Color hoverEmpty;
};

void updateColors(Colors* colors);

int renderSettings(
    sf::RenderWindow* window,
    sf::Font font,
    sf::Vector2f mousePosition,
    bool isMouseClicked,
    bool isMouseHeld,
    Colors colors,
    int* difficultyLevel
);
