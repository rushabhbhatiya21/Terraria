#include "gameMain.h"
#include <gameplay.h>
#include <assetManager.h>
#include <audio.h>
#include <settings.h>
#include <ui.h>
#include <drawBackground.h>
#include <iostream>

#pragma region global variables

AssetManager assetManager;
Gameplay gameplay;
UIEngine mainMenuButtons;
DrawBackground backgroundForMenu;
// set false for menu
bool gameplayRunning = true;

#pragma endregion


bool initGame()
{
	//Audio::init();
	assetManager.loadAll();
	assetManager.loadTexturePack("hdtextures");
	loadSettings();

	gameplay.init(assetManager);

	return true;
}

bool updateGame()
{
	//Audio::update();
	updateSettings();

	ClearBackground({ 0,0,0,255 });

	if (!gameplayRunning)
	{
		Camera2D c = {};
		c.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };
		c.target = Vector2{ 500,500 };
		c.zoom = 20;
		backgroundForMenu.draw(GetFrameTime(), assetManager, c, { 1000,1000 }, WHITE);

		mainMenuButtons.addTitle("Canvas Adventures");

		if (mainMenuButtons.addButton("Start Game"))
		{
			gameplayRunning = true;
			//gameplay = {};
			// todo: implement gameplay.reset() and call here
			gameplay.init(assetManager);
		}

		mainMenuButtons.addButton("Settings");

		if (mainMenuButtons.addButton("Exit"))
		{
			return false;
		}

		mainMenuButtons.updateAndRender();
		return true;
	}
	else
	{
		return gameplay.update(assetManager);
	}

	return true;
}

void closeGame()
{
	gameplay.closeGame(assetManager);
	printf("\n\nCLOSED!!!!!!!!!\n\n");
}