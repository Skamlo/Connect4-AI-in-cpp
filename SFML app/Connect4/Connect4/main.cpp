#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include "Ai.h"
#include "Board.h"
#include "Menu.h"
#include "Player.h"
#include "Settings.h"

#define MENU 1
#define PLAYERvsPLAYER 2
#define PLAYERvsAI 3
#define SETTINGS 4
#define EXIT 5

// Functions declaration
void update(sf::RenderWindow* window);
void render(sf::RenderWindow* window);

// Window params
int fps = 144;
int width = 1000;
int height = 800;
int currentPage = MENU;

// Mouse
sf::Vector2f mousePosition;
bool isMouseClicked = false;
bool isMouseHeld = false;

// Other
sf::Font font;
sf::Texture logo;
Colors colors;

// Settings
int difficultyLevel = 8;
bool sliderIsHolding = false;

int main()
{
    // Window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "Connect 4", sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(fps);

    // Window icon
    sf::Image icon;
    if (!icon.loadFromFile("img/icon.png"))
        std::cout << "ERROR: Failed during loading icon.png file" << "\n";
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    if (!font.loadFromFile("fonts/Lato-Regular.ttf"))
        std::cout << "ERROR: Failed during loading Lato-Regular.ttf file" << "\n";
    if (!logo.loadFromFile("img/logo.png"))
        std::cout << "ERROR: Failed during loading logo.png file" << "\n";
    updateColors(&colors);

    // Main loop
    while (window.isOpen())
    {
        if (currentPage == EXIT)
        {
            window.close();
            break;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                width = event.size.width;
                height = event.size.height;
                sf::FloatRect visibleArea(0, 0, width, height);
                window.setView(sf::View(visibleArea));
                break;
            }
        }

        // Clearing window
        window.clear(colors.backgroundColor);

        update(&window);
        render(&window);

        // Displaing
        window.display();
    }

	return 0;
}


void update(sf::RenderWindow* window)
{
    // Mouse
    updateMousePosition(window, &mousePosition);
    updateMouseClick(&isMouseClicked, &isMouseHeld);
}


void render(sf::RenderWindow* window)
{
    switch (currentPage)
    {
    case MENU:
        currentPage = renderMenu(
            window,
            font,
            mousePosition,
            isMouseClicked,
            isMouseHeld,
            colors,
            logo
        );
        break;

    case PLAYERvsPLAYER:
        currentPage = renderBoard(window);
        break;

    case PLAYERvsAI:
        currentPage = renderBoard(window);
        break;

    case SETTINGS:
        currentPage = renderSettings(
            window,
            font,
            mousePosition,
            isMouseClicked,
            isMouseHeld,
            colors,
            &difficultyLevel,
            &sliderIsHolding
        );
        break;

    default:
        break;
    }
}
