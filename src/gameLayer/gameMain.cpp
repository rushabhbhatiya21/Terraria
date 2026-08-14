#include "gameMain.h"
#include <math/vec2.h>
#include <math/cam.h>
#include <window/window.h>
#include <time/time.h>

#include <gameplay.h>
#include <assets/assetManager.h>
#include <audio.h>
#include <settings.h>
#include <ui.h>
#include <drawBackground.h>
#include <saveMap.h>

#pragma region global variables

Engine::AssetManager assetManager;
Gameplay gameplay;
UIEngine mainMenuButtons;
DrawBackground backgroundForMenu;
// set false for menu
bool gameplayRunning = false;

static void startNewGame()
{
	gameplay.init(assetManager);
	gameplayRunning = true;
}

#pragma endregion


bool initGame()
{
	Audio::init();
	assetManager.loadAll();
	assetManager.loadTexturePack("hdtextures");
	mainMenuButtons.init();
	loadSettings();

	gameplay.init(assetManager);

	return true;
}

bool updateGame()
{
	Audio::update();
	updateSettings();

	//ClearBackground({ 0,0,0,255 });

	if (!gameplayRunning)
	{
		Engine::Cam c = {};
		c.offset = { Engine::getScreenWidth() / 2.f, Engine::getScreenHeight() / 2.f };
		c.target = Engine::Vec2{ 500,500 };
		c.zoom = 20;
		backgroundForMenu.draw(Engine::getFrameTime(), assetManager, c, {1000,1000}, Engine::White, gameplay.sceneRenderer);

		mainMenuButtons.addTitle("Terraframe");

		if (mainMenuButtons.addButton("Resume Game"))
		{
			if (loadWorld(gameplay.gameMap, gameplay.entityHolder, gameplay.player))
			{
				gameplayRunning = true;
			}
			else
			{
				Engine::traceLog(Engine::LogLevel::Warning, "Resume failed: no valid save found.");
			}
		}

		if (mainMenuButtons.addButton("New Game"))
		{
			startNewGame();
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