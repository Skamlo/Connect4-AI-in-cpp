#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#define ROW_COUNT 6
#define COLUMN_COUNT 7

int firstMove(int(*board)[COLUMN_COUNT]);
int randInt(int low, int high);
int randChoice(int validLocations[COLUMN_COUNT]);
int getNextOpenRow(int(*board)[COLUMN_COUNT], int col);
void copyBoard(int(*board)[COLUMN_COUNT], int(*boardCopied)[COLUMN_COUNT]);
int oppositPiece(int player);
double windowEvaluate(int window[4], int piece);
double scorePosition(int(*board)[COLUMN_COUNT], int piece);
int getColFromScore(double scores[COLUMN_COUNT], double targetValue);
bool isTerminalNode(int(*board)[COLUMN_COUNT]);
double* minimax(int(*board)[COLUMN_COUNT], int depth, double alpha, double beta, bool maximizingPlayer);
