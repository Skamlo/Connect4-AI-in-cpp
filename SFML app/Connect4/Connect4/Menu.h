#pragma once

#include "Settings.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

void drawButton(sf::RenderWindow* window, float xPos, float yPos, sf::Color color, sf::Text text);

int renderMenu(sf::RenderWindow* window, sf::Font font, sf::Vector2f mousePosition, bool isMouseClicked, bool isMouseHeld, Colors colors, sf::Texture logo);
 