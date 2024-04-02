#include "Player.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

void updateMousePosition(sf::RenderWindow* window, sf::Vector2f* mousePosition)
{
	sf::Vector2i mousePosWindow = sf::Mouse::getPosition(*window);
	*mousePosition = window->mapPixelToCoords(mousePosWindow);
}

void updateMouseClick(bool* isMouseClicked, bool* isMouseHeld)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (*isMouseHeld)
		{
			*isMouseClicked = false;
		}
		else
		{
			*isMouseClicked = true;
			*isMouseHeld = true;
		}
	}
	else
	{
		*isMouseClicked = false;
		*isMouseHeld = false;
	}
}
