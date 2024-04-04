#include "Settings.h"
#include "Menu.h"

#include <iostream>
#include <fstream>
#include <string>

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

#define MIN_DIFFICULTY_LEVEL 0
#define MAX_DIFFICULTY_LEVEL 10

void updateColors(Colors* colors)
{
    colors->backgroundColor = sf::Color::Color(22, 48, 39, 255);
    colors->hoverBackgroundColor = sf::Color::Color(34, 74, 60, 255);
    colors->foregroundColor = sf::Color::Color(17, 143, 99, 255);
    colors->hoverForegroundColor = sf::Color::Color(21, 173, 120, 255);
    colors->firstPlayerColor = sf::Color::Color(219, 22, 22, 255);
    colors->hoverFirstPlayerColor = sf::Color::Color(214, 75, 75, 255);
    colors->secondPlayerColor = sf::Color::Color(159, 201, 32, 255);
    colors->hoverSecondPlayerColor = sf::Color::Color(169, 196, 86, 255);
    colors->white = sf::Color::Color(247, 247, 247, 255);
    colors->dirtyWhite = sf::Color(247, 247, 247, 255);
    colors->empty = sf::Color::Color(34, 74, 60, 255);
    colors->hoverEmpty = sf::Color::Color(43, 94, 77, 255);
}


void readNameAndValue(std::string& line, std::string& name, std::string& value)
{
    size_t start_pos = line.find('"');
    if (start_pos != std::string::npos)
    {
        size_t end_pos = line.find('"', start_pos + 1);
        if (end_pos != std::string::npos)
        {
            name = line.substr(start_pos + 1, end_pos - start_pos - 1);
            size_t colon_pos = line.find(':', end_pos);
            if (colon_pos != std::string::npos)
            {
                value = line.substr(colon_pos + 1);
                // Trim leading and trailing whitespaces from the value
                value.erase(0, value.find_first_not_of(" \n\r\t"));
                value.erase(value.find_last_not_of(" \n\r\t") + 1);
            }
        }
    }
}


void saveSettings(int difficultyLevel)
{
    std::ofstream file("settings/settings.json");

    if (file.is_open())
    {
        file << "{\"settings\": {" << std::endl
            << "\t\"difficultyLevel\": " << difficultyLevel << std::endl
            << "}}";
    }
    else
    {
        std::cout << "SETTINGS::SAVE_SETTINGS::IS_FILE_OPEN::Failed   Can't open settings.json file" << std::endl;
    }

    file.close();
}


void readSettings(int* difficultyLevel)
{
    std::ifstream file("settings/settings.json");

    if (file.is_open())
    {
        int nParams = 1;

        std::string line;
        std::getline(file, line);

        for (int i = 0; i < nParams; i++)
        {
            std::getline(file, line);
            std::string name, valueString;
            readNameAndValue(line, name, valueString);

            int valueInt = std::stoi(valueString);

            if (name == "difficultyLevel")
                *difficultyLevel = valueInt;
        }
    }
    else
    {
        std::cout << "SETTINGS::READ_SETTINGS::IS_FILE_OPEN::Failed   Can't open settings.json file" << std::endl;
    }

    file.close();
}


void drawSlider(sf::RenderWindow* window, sf::Font font, Colors colors, int difficultyLevel, float xPos, float yPos)
{
    // Red background
    sf::RectangleShape backgroundRectangle(sf::Vector2f(500, 26));
    backgroundRectangle.setFillColor(colors.foregroundColor);
    backgroundRectangle.setPosition(xPos + 13, yPos);
    sf::CircleShape backgroundcircleLeft(13);
    backgroundcircleLeft.setFillColor(colors.foregroundColor);
    backgroundcircleLeft.setPosition(xPos, yPos);
    sf::CircleShape backgroundcircleRight(13);
    backgroundcircleRight.setFillColor(colors.foregroundColor);
    backgroundcircleRight.setPosition(xPos + 500, yPos);
    window->draw(backgroundRectangle);
    window->draw(backgroundcircleLeft);
    window->draw(backgroundcircleRight);

    // White foreground
    sf::RectangleShape foregroundRectangle(sf::Vector2f(500, 16));
    foregroundRectangle.setFillColor(colors.white);
    foregroundRectangle.setPosition(xPos + 13, yPos + 5);
    sf::CircleShape foregroundcircleLeft(8);
    foregroundcircleLeft.setFillColor(colors.white);
    foregroundcircleLeft.setPosition(xPos + 5, yPos + 5);
    sf::CircleShape foregroundcircleRight(8);
    foregroundcircleRight.setFillColor(colors.white);
    foregroundcircleRight.setPosition(xPos + 505, yPos + 5);
    window->draw(foregroundRectangle);
    window->draw(foregroundcircleLeft);
    window->draw(foregroundcircleRight);

    // Ball on slider
    float x = 500 * difficultyLevel / MAX_DIFFICULTY_LEVEL;
    sf::CircleShape backgroundBall(16);
    backgroundBall.setFillColor(colors.hoverForegroundColor);
    backgroundBall.setPosition(xPos - 3 + x, yPos - 3);
    sf::CircleShape foregroundBall(13);
    foregroundBall.setFillColor(colors.dirtyWhite);
    foregroundBall.setPosition(xPos + x, yPos);
    window->draw(backgroundBall);
    window->draw(foregroundBall);

    // Number over ball
    sf::Text difficultyLevelText("None", font, 18);
    if (difficultyLevel == 0)
        difficultyLevelText.setString("Random choices");
    else
        difficultyLevelText.setString(std::to_string(difficultyLevel));
    float textWdith = difficultyLevelText.getLocalBounds().width;
    difficultyLevelText.setPosition(xPos + x + 13 - (textWdith / 2), yPos - 29);
    window->draw(difficultyLevelText);
}


int renderSettings(sf::RenderWindow* window, sf::Font font, sf::Vector2f mousePosition, bool isMouseClicked, bool isMouseHeld, Colors colors, int* difficultyLevel)
{
    int output = SETTINGS;

    float width = static_cast<float>(window->getSize().x);
    float height = static_cast<float>(window->getSize().y);

    float yPadding = (height - 308) / 2;
    if (yPadding < 10)
        yPadding = 10;

    // Informations for slider
    sf::Text infoForSlider1("Set AI difficulty level", font, 24); // height 21
    sf::Text infoForSlider2("Remember, the higher the number, the", font, 18); // height 16
    sf::Text infoForSlider3("more computing resources you need.", font, 18);
    sf::Text infoForSlider4("Recommended value is 8.", font, 18);

    infoForSlider1.setPosition((width - infoForSlider1.getLocalBounds().width) / 2, yPadding);
    infoForSlider2.setPosition((width - infoForSlider2.getLocalBounds().width) / 2, yPadding + 26);
    infoForSlider3.setPosition((width - infoForSlider3.getLocalBounds().width) / 2, yPadding + 47);
    infoForSlider4.setPosition((width - infoForSlider4.getLocalBounds().width) / 2, yPadding + 68);

    window->draw(infoForSlider1);
    window->draw(infoForSlider2);
    window->draw(infoForSlider3);
    window->draw(infoForSlider4);

    // Slider
    float xPosSlider = (width - 526) / 2;
    if (xPosSlider < 10)
        xPosSlider = 10;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (mousePosition.x >= xPosSlider && mousePosition.x <= xPosSlider + 526 &&
            mousePosition.y >= yPadding + 168 && mousePosition.y <= yPadding + 168 + 26)
        {
            if (mousePosition.x <= xPosSlider + 13)
                *difficultyLevel = 0;
            else if (mousePosition.x >= xPosSlider + 513)
                *difficultyLevel = MAX_DIFFICULTY_LEVEL;
            else
                *difficultyLevel = static_cast<int>(round((mousePosition.x - xPosSlider - 13) / 500 * MAX_DIFFICULTY_LEVEL));

            saveSettings(*difficultyLevel);
        }
    }

    drawSlider(window, font, colors, *difficultyLevel, xPosSlider, yPadding + 168);

    // Back button
    float xPosButton = (width - 200) / 2;
    sf::Text back("Back", font, 24);
    if (mousePosition.x >= xPosButton && mousePosition.x <= xPosButton + 200 &&
        mousePosition.y >= yPadding + 244 && mousePosition.y <= yPadding + 244 + 48)
    {
        drawButton(window, xPosButton, yPadding + 244, colors.hoverForegroundColor, back);
        if (isMouseClicked) output = MENU;
    }
    else
    {
        drawButton(window, xPosButton, yPadding + 244, colors.foregroundColor, back);
    }

    return output;
}
