#include "Board.h"
#include "Settings.h"
#include "Ai.h"
#include "Menu.h"

#include <iostream>
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

#define EMPTY 0
#define FIRST_PLAYER 1
#define SECOND_PLAYER 2

#define ROW_COUNT 6
#define COLUMN_COUNT 7

#define GAME 1
#define FIRST_PLAYER_WIN 2
#define SECOND_PLAYER_WIN 3
#define DRAW 4

#define LOW_VALUE -1000000000
#define HIGH_VALUE 1000000000

#define PI 3.141592653

const int fps = 144;
const float endSecondsWaiting = 3.0;

void resetBoard(int (*board)[COLUMN_COUNT])
{
	for (int r = 0; r < ROW_COUNT; r++)
	{
		for (int c = 0; c < COLUMN_COUNT; c++)
		{
			board[r][c] = EMPTY;
		}
	}
}


bool isValidColumn(int(*board)[COLUMN_COUNT], int column)
{
	if (board[ROW_COUNT - 1][column] == EMPTY)
		return true;
	else
		return false;
}


int* getValidLocations(int(*board)[COLUMN_COUNT])
{
	static int validLocations[COLUMN_COUNT];
	for (int c = 0; c < COLUMN_COUNT; c++) {
		if (isValidColumn(board, c))
			validLocations[c] = true;
		else
			validLocations[c] = false;
	}
	return validLocations;
}


void dropPiece(int(*board)[COLUMN_COUNT], int column, int piece)
{
	if (isValidColumn(board, column)) {
		for (int r = 0; r < ROW_COUNT; r++) {
			if (board[r][column] == EMPTY) {
				board[r][column] = piece;
				break;
			}
		}
	}
}


int winningWindow(int window[4], int piece)
{
	for (int i = 0; i < 4; i++) {
		if (window[i] != piece) {
			return false;
		}
	}
	return true;
}


bool winningMove(int(*board)[COLUMN_COUNT], int piece)
{
	int window[4];

	// horizontal
	for (int r = 0; r < ROW_COUNT; r++) {
		for (int c = 0; c < COLUMN_COUNT - 3; c++) {
			for (int i = 0; i < 4; i++) window[i] = board[r][c + i];
			if (winningWindow(window, piece)) return true;
		}
	}

	// vertivcal
	for (int r = 0; r < ROW_COUNT - 3; r++) {
		for (int c = 0; c < COLUMN_COUNT; c++) {
			for (int i = 0; i < 4; i++) window[i] = board[r + i][c];
			if (winningWindow(window, piece)) return true;
		}
	}

	// diagonal
	for (int r = 0; r < ROW_COUNT - 3; r++) {
		for (int c = 0; c < COLUMN_COUNT - 3; c++) {
			// diagonal left
			for (int i = 0; i < 4; i++) window[i] = board[r + i][c + i];
			if (winningWindow(window, piece)) return true;
			// diagonal right
			for (int i = 0; i < 4; i++) window[i] = board[r + i][c + 3 - i];
			if (winningWindow(window, piece)) return true;
		}
	}
	return false;
}


bool isDraw(int(*board)[COLUMN_COUNT])
{
	for (int c = 0; c < COLUMN_COUNT; c++) {
		if (isValidColumn(board, c)) return false;
	}
	return true;
}


void drawInfoBackground(sf::RenderWindow* window, int xPos, int yPos, int infoWidth, int infoHeight, sf::Color color, int radius)
{
	sf::CircleShape circleLeftTop(radius);
	sf::CircleShape circleRightTop(radius);
	sf::CircleShape circleLeftBottom(radius);
	sf::CircleShape circleRightBottom(radius);
	sf::RectangleShape rectangleVertical(sf::Vector2f(infoWidth - 2 * radius, infoHeight));
	sf::RectangleShape rectangleHorizontal(sf::Vector2f(infoWidth, infoHeight - 2 * radius));

	circleLeftTop.setFillColor(color);
	circleRightTop.setFillColor(color);
	circleLeftBottom.setFillColor(color);
	circleRightBottom.setFillColor(color);
	rectangleVertical.setFillColor(color);
	rectangleHorizontal.setFillColor(color);

	circleLeftTop.setPosition(xPos, yPos);
	circleRightTop.setPosition(xPos + infoWidth - 2 * radius, yPos);
	circleLeftBottom.setPosition(xPos, yPos + infoHeight - 2 * radius);
	circleRightBottom.setPosition(xPos + infoWidth - 2 * radius, yPos + infoHeight - 2 * radius);
	rectangleVertical.setPosition(xPos + radius, yPos);
	rectangleHorizontal.setPosition(xPos, yPos + radius);

	window->draw(circleLeftTop);
	window->draw(circleRightTop);
	window->draw(circleLeftBottom);
	window->draw(circleRightBottom);
	window->draw(rectangleVertical);
	window->draw(rectangleHorizontal);
}


void drawBoard(sf::RenderWindow* window, int (*board)[COLUMN_COUNT], int xPos, int yPos, int squareSize, sf::Vector2f mousePosition, Colors colors, int* state)
{
	for (int c = 0; c < COLUMN_COUNT; c++)
	{
		bool mouseOnColumn = false;
		if (mousePosition.x >= xPos + squareSize * c && mousePosition.x < xPos + squareSize * (c + 1) &&
			mousePosition.y >= yPos && mousePosition.y <= yPos + squareSize * 6)
			mouseOnColumn = true;

		for (int r = ROW_COUNT - 1; r >= 0; r--)
		{
			sf::CircleShape circle(static_cast<float>(squareSize) * 0.8 / 2);
			float squarePadding = static_cast<float>(squareSize) * 0.1;
			circle.setPosition(xPos + squareSize * c + squarePadding, yPos + squareSize * (ROW_COUNT-r-1) + squarePadding);

			if (board[r][c] == EMPTY)
			{
				if (mouseOnColumn && *state == GAME)
					circle.setFillColor(colors.hoverEmpty);
				else
					circle.setFillColor(colors.empty);
			}
			else if (board[r][c] == FIRST_PLAYER)
			{
				if (mouseOnColumn && *state == GAME)
					circle.setFillColor(colors.hoverFirstPlayerColor);
				else
					circle.setFillColor(colors.firstPlayerColor);
			}
			else if (board[r][c] == SECOND_PLAYER)
			{
				if (mouseOnColumn && *state == GAME)
					circle.setFillColor(colors.hoverSecondPlayerColor);
				else
					circle.setFillColor(colors.secondPlayerColor);
			}

			window->draw(circle);
		}
	}
}


int updateState(int(*board)[COLUMN_COUNT])
{
	if (winningMove(board, FIRST_PLAYER))
		return FIRST_PLAYER_WIN;
	if (winningMove(board, SECOND_PLAYER))
		return SECOND_PLAYER_WIN;
	if (isDraw(board))
		return DRAW;
	else
		return GAME;
}


int renderBoard(sf::RenderWindow* window, int (*board)[COLUMN_COUNT], sf::Font font, sf::Vector2f mousePosition, bool isMouseClicked, bool isMouseHeld, int* currentPlayer, Colors colors, int* difficultyLevel, int currentPage, int* state, int* waitTimer)
{
	int output = currentPage;

	float width = static_cast<float>(window->getSize().x);
	float height = static_cast<float>(window->getSize().y);

	// board rendering
	int squareSize = static_cast<int>((height - 168) / ROW_COUNT);
	if ((width - 20) / COLUMN_COUNT < squareSize) squareSize = static_cast<int>((width - 20) / COLUMN_COUNT);
	int xPos = static_cast<int>((width - squareSize * COLUMN_COUNT) / 2);
	int yPos = static_cast<int>((height - 68 - squareSize * ROW_COUNT) / 2);
	drawBoard(window, board, xPos, yPos, squareSize, mousePosition, colors, state);

	// Get decision from bourd
	if (isMouseClicked &&
		mousePosition.x >= xPos && mousePosition.x < xPos + squareSize * COLUMN_COUNT &&
		mousePosition.y >= yPos && mousePosition.y < yPos + squareSize * ROW_COUNT &&
		*state == GAME && *waitTimer == 0)
	{
		int col = floor((mousePosition.x - xPos) / squareSize);

		if (currentPage == PLAYERvsPLAYER)
		{
			dropPiece(board, col, *currentPlayer);

			// Change player
			if (*currentPlayer == FIRST_PLAYER)
				*currentPlayer = SECOND_PLAYER;
			else if (*currentPlayer == SECOND_PLAYER)
				*currentPlayer = FIRST_PLAYER;

			*state = updateState(board);
			if (*state != GAME)
				*waitTimer = endSecondsWaiting * fps;
		}
		else if (currentPage == PLAYERvsAI)
		{
			dropPiece(board, col, FIRST_PLAYER);
			
			*currentPlayer = SECOND_PLAYER;

			*state = updateState(board);
			if (*state != GAME)
				*waitTimer = endSecondsWaiting * fps;
			else
				*waitTimer = fps / 2;
		}
	}

	// AI move
	if (currentPage == PLAYERvsAI && *waitTimer == 1 && *state == GAME)
	{
		int col = firstMove(board);
		if (col == 0)
		{
			if (*difficultyLevel == 0)
			{
				col = randChoice(getValidLocations(board));
			}
			else
			{
				col = static_cast<int>(minimax(
					board,
					*difficultyLevel,
					static_cast<double>(LOW_VALUE),
					static_cast<double>(HIGH_VALUE),
					true
				)[0]);
			}
		}

		dropPiece(board, col, SECOND_PLAYER);

		*currentPlayer = FIRST_PLAYER;

		// Update game state
		*state = updateState(board);
		if (*state != GAME)
			*waitTimer = endSecondsWaiting * fps;
	}

	// Player state
	if (*state == GAME)
	{
		sf::CircleShape circle(23);
		circle.setPosition(width - 66, height - 66);
		if (*currentPlayer == FIRST_PLAYER)
			circle.setFillColor(colors.firstPlayerColor);
		else if (*currentPlayer == SECOND_PLAYER)
			circle.setFillColor(colors.secondPlayerColor);
		window->draw(circle);

		sf::Text currentPlayerText("None", font, 24);
		if (currentPage == PLAYERvsPLAYER)
		{
			if (*currentPlayer == FIRST_PLAYER)
				currentPlayerText.setString("First player");
			else if (*currentPlayer == SECOND_PLAYER)
				currentPlayerText.setString("Second player");
		}
		else if (currentPage == PLAYERvsAI)
		{
			if (*currentPlayer == FIRST_PLAYER)
				currentPlayerText.setString("You");
			else if (*currentPlayer == SECOND_PLAYER)
				currentPlayerText.setString("AI");
		}

		float xPos = width - 76 - currentPlayerText.getLocalBounds().width;
		float yPos = height - 58;
		currentPlayerText.setPosition(xPos, yPos);
		window->draw(currentPlayerText);
	}
	else
	{
		int xPos = width - 86;
		int yPos = height - 86;

		// Background circle
		sf::CircleShape backgroundCircle(23);
		backgroundCircle.setPosition(xPos + 20, yPos + 20);
		backgroundCircle.setFillColor(colors.foregroundColor);
		window->draw(backgroundCircle);

		// Foreground circle
		sf::CircleShape foregroundCircle(18);
		foregroundCircle.setPosition(xPos + 25, yPos + 25);
		foregroundCircle.setFillColor(colors.backgroundColor);
		window->draw(foregroundCircle);

		// Loading polygon
		float fillRato = static_cast<float>(*waitTimer) / (endSecondsWaiting * fps);

		sf::ConvexShape convex;
		convex.setFillColor(colors.backgroundColor);
		convex.setPointCount(8);
		convex.setPoint(0, sf::Vector2f(xPos + 43, yPos + 43));
		convex.setPoint(1, sf::Vector2f(xPos + 43, yPos));
		for (int i = 2; i < 8; i++)
		{
			float partFillRato = 1 - fillRato;
			partFillRato = partFillRato / 6 * (i - 1);
			int xRato = static_cast<int>(43 * sin(partFillRato * 2 * PI));
			int yRato = -static_cast<int>(43 * cos(partFillRato * 2 * PI));
			convex.setPoint(i, sf::Vector2f(xPos + 43 + xRato, yPos + 43 + yRato));
		}
		window->draw(convex);
	}

	// Back button
	sf::Text backText("Back", font, 24);
	if (mousePosition.x >= 20 && mousePosition.x <= 220 &&
		mousePosition.y >= height - 68 && mousePosition.y < height - 20)
	{
		if (*state == GAME)
		{
			drawButton(window, 20, height - 68, colors.hoverForegroundColor, backText);
			if (isMouseClicked)
				output = MENU;
		}
		else
			drawButton(window, 20, height - 68, colors.foregroundColor, backText);
	}	
	else
	{
		drawButton(window, 20, height - 68, colors.foregroundColor, backText);
	}

	// Draw game end
	if (*state != GAME && *waitTimer == 0)
	{
		int infoWidth = static_cast<int>(width / 3);
		if (infoWidth < 220)
			infoWidth = 220;
		int infoHeight = static_cast<int>(height / 3);
		if (infoHeight < 117)
			infoHeight = 117;
		int xPos = static_cast<int>((width - infoWidth) / 2);
		int yPos = static_cast<int>((height - infoHeight) / 2);

		// Background
		drawInfoBackground(window, xPos, yPos, infoWidth, infoHeight, colors.hoverEmpty, 25);

		// Info
		sf::Text endInfo("None", font, 24);
		if (*state == FIRST_PLAYER_WIN)
		{
			if (currentPage == PLAYERvsPLAYER)
				endInfo.setString("First player won!");
			else if (currentPage == PLAYERvsAI)
				endInfo.setString("You won!");
		}
		else if (*state == SECOND_PLAYER_WIN)
		{
			if (currentPage == PLAYERvsPLAYER)
				endInfo.setString("Second player won!");
			else if (currentPage == PLAYERvsAI)
				endInfo.setString("AI won!");
		}
		else if (*state == DRAW)
			endInfo.setString("Draw!");

		int xPosInfo = xPos + (infoWidth - endInfo.getLocalBounds().width) / 2;
		int yPosInfo = yPos + (infoHeight - 117) / 2;
		endInfo.setPosition(xPosInfo, yPosInfo);
		window->draw(endInfo);

		// Button
		int xPosButton = xPos + (infoWidth - 200) / 2;
		int yPosButton = yPos + (infoHeight - 117) / 2 + 51;

		sf::Text infoBackText("Back to menu", font, 24);
		if (mousePosition.x >= xPosButton && mousePosition.x <= xPosButton + 200 &&
			mousePosition.y >= yPosButton && mousePosition.y <= yPosButton + 46)
		{
			drawButton(window, xPosButton, yPosButton, colors.hoverForegroundColor, backText);
			if (isMouseClicked)
				output = MENU;
		}
		else
		{
			drawButton(window, xPosButton, yPosButton, colors.foregroundColor, backText);
		}
	}

	// Decrese wait timer
	if (*waitTimer > 0)
		*waitTimer = *waitTimer - 1;

	return output;
}
