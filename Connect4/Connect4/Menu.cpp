#include "Menu.h"
#include "Settings.h"

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#define MENU 1
#define PLAYERvsPLAYER 2
#define PLAYERvsAI 3
#define SETTINGS 4
#define EXIT 5

void drawButton(sf::RenderWindow* window, float xPos, float yPos, sf::Color color, sf::Text text)
{
	sf::CircleShape circleLeftTop(10);
	sf::CircleShape circleRightTop(10);
	sf::CircleShape circleLeftBottom(10);
	sf::CircleShape circleRightBottom(10);
	sf::RectangleShape rectangleVertical(sf::Vector2f(180, 48));
	sf::RectangleShape rectangleHorizontal(sf::Vector2f(200, 28));

	circleLeftTop.setFillColor(color);
	circleRightTop.setFillColor(color);
	circleLeftBottom.setFillColor(color);
	circleRightBottom.setFillColor(color);
	rectangleVertical.setFillColor(color);
	rectangleHorizontal.setFillColor(color);

	circleLeftTop.setPosition(xPos, yPos);
	circleRightTop.setPosition(xPos+180, yPos);
	circleLeftBottom.setPosition(xPos, yPos+28);
	circleRightBottom.setPosition(xPos+180, yPos+28);
	rectangleVertical.setPosition(xPos+10, yPos);
	rectangleHorizontal.setPosition(xPos, yPos+10);

	window->draw(circleLeftTop);
	window->draw(circleRightTop);
	window->draw(circleLeftBottom);
	window->draw(circleRightBottom);
	window->draw(rectangleVertical);
	window->draw(rectangleHorizontal);

	float textWidth = text.getLocalBounds().width;
	text.setPosition((200 - textWidth) / 2 + xPos, 8 + yPos);
	window->draw(text);
}

int renderMenu(sf::RenderWindow* window, sf::Font font, sf::Vector2f mousePosition, bool isMouseClicked, bool isMouseHeld, Colors colors, sf::Texture logo)
{
	int output = MENU;

	float width = static_cast<float>(window->getSize().x);
	float height = static_cast<float>(window->getSize().y);
	
	sf::Text pvspText("Player vs Player", font, 24);
	sf::Text pvsaiText("Player vs AI", font, 24);
	sf::Text settingsText("Settings", font, 24);
	sf::Text exitText("Exit", font, 24);

	float xPadding = (width - 200) / 2;
	if (xPadding < 10)
		xPadding = 10;
	float yPadding = (height - 472) / 2;
	if (yPadding < 10)
		yPadding = 10;

	sf::Color color;

	// Logo
	sf::Text logoText("Connect 4", font, 72);
	float logoTextWdith = logoText.getLocalBounds().width;
	float logoTextHeight = logoText.getLocalBounds().height;
	logoText.setPosition(
		(width - (220 + logoTextWdith)) / 2 + 220,
		(200 - logoTextHeight) / 2 + yPadding
	);

	sf::Sprite logoSprite(logo);
	logoSprite.setPosition(
		(width - (220 + logoTextWdith)) / 2,
		yPadding
	);

	window->draw(logoSprite);
	window->draw(logoText);

	// Player vs player
	if (mousePosition.x >= xPadding && mousePosition.x <= xPadding + 200 && mousePosition.y >= yPadding + 250 && mousePosition.y <= yPadding + 250 + 48)
	{
		color = colors.hoverForegroundColor;
		if (isMouseClicked)
			output = PLAYERvsPLAYER;
	}
	else
		color = colors.foregroundColor;
	drawButton(window, xPadding, yPadding + 250, color, pvspText);

	// Player vs Ai
	if (mousePosition.x >= xPadding && mousePosition.x <= xPadding + 200 && mousePosition.y >= yPadding + 250 + 58 && mousePosition.y <= yPadding + 250 + 58 + 48)
	{
		color = colors.hoverForegroundColor;
		if (isMouseClicked)
			output = PLAYERvsAI;
	}
	else
		color = colors.foregroundColor;
	drawButton(window, xPadding, yPadding + 250 + 58, color, pvsaiText);

	// Settings
	if (mousePosition.x >= xPadding && mousePosition.x <= xPadding + 200 && mousePosition.y >= yPadding + 250 + 116 && mousePosition.y <= yPadding + 250 + 116 + 48)
	{
		color = colors.hoverForegroundColor;
		if (isMouseClicked)
			output = SETTINGS;
	}
	else
		color = colors.foregroundColor;
	drawButton(window, xPadding, yPadding + 250 + 116, color, settingsText);

	// Exit
	if (mousePosition.x >= xPadding && mousePosition.x <= xPadding + 200 && mousePosition.y >= yPadding + 250 + 174 && mousePosition.y <= yPadding + 250 + 174 + 48)
	{
		color = colors.hoverForegroundColor;
		if (isMouseClicked)
			output = EXIT;
	}
	else
		color = colors.foregroundColor;
	drawButton(window, xPadding, yPadding + 250 + 174, color, exitText);

	return output;
}
