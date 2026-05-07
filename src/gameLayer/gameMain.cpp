#include "gameMain.h"
#include <gameplay.h>
#include <assetManager.h>
#include <audio.h>
#include <settings.h>
#include <ui.h>
#include <drawBackground.h>

#pragma region global variables

AssetManager assetManager;
Gameplay gameplay;
UIEngine mainMenuButtons;
DrawBackground backgroundForMenu;

#pragma endregion


bool initGame()
{
	Audio::init();
	assetManager.loadAll();
	loadSettings();

	gameplay.init();

	return true;
}

bool updateGame()
{
	ClearBackground({ 0,0,0,255 });

	Camera2D c = {};
	c.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };
	c.target = Vector2{ 500,500 };
	c.zoom = 20;
	backgroundForMenu.draw(GetFrameTime(), assetManager, c, { 1000,1000 });

	mainMenuButtons.addTitle("hello");
	mainMenuButtons.addButton("Button");
	mainMenuButtons.addButton("Button");

	mainMenuButtons.updateAndRender();
	//return gameplay.update(assetManager);
	return true;
}

void closeGame()
{
	gameplay.closeGame();
	printf("\n\nCLOSED!!!!!!!!!\n\n");
}