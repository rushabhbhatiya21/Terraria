#include <iostream>
#include <fstream>
#include <raylib.h>
#include "gameMain.h"

struct GameData
{
	float posX = 100;
	float posY = 100;
} gameData;

bool initGame()
{
	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5.f) deltaTime = 1 / 5.f;

	DrawRectangle(gameData.posX, gameData.posY, 50, 50, { 255,0,200,255 });

	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) gameData.posY -= 200.f * deltaTime;
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) gameData.posY += 200.f * deltaTime;
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) gameData.posX -= 200.f * deltaTime;
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) gameData.posX += 200.f * deltaTime;

	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}