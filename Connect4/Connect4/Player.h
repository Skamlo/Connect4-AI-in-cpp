#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

void updateMousePosition(sf::RenderWindow* window, sf::Vector2f* mousePosition);
void updateMouseClick(bool* isMouseClicked, bool* isMouseHeld);
