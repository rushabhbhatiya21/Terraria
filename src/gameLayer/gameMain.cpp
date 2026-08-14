#include "gameMain.h"
#include <math/vec2.h>
#include <math/cam.h>
#include <window/window.h>
#include <time/time.h>
#include <input/input.h>

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
bool pausedFromGameplay = false;

enum class MenuPage
{
	Main,
	Settings
};

MenuPage menuPage = MenuPage::Main;

static void startNewGame()
{
	gameplay.init(assetManager);
	gameplayRunning = true;
	pausedFromGameplay = false;
	menuPage = MenuPage::Main;
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

	if (!gameplayRunning && pausedFromGameplay && Engine::isKeyPressed(Engine::Key::Escape))
	{
		gameplayRunning = true;
		pausedFromGameplay = false;
		menuPage = MenuPage::Main;
		return true;
	}

	if (gameplayRunning && Engine::isKeyPressed(Engine::Key::Escape))
	{
		gameplayRunning = false;
		pausedFromGameplay = true;
		menuPage = MenuPage::Main;
		return true;
	}

	//ClearBackground({ 0,0,0,255 });

	if (!gameplayRunning)
	{
		Engine::Cam c = {};
		c.offset = { Engine::getScreenWidth() / 2.f, Engine::getScreenHeight() / 2.f };
		c.target = Engine::Vec2{ 500,500 };
		c.zoom = 20;
		backgroundForMenu.draw(Engine::getFrameTime(), assetManager, c, {1000,1000}, Engine::White, gameplay.sceneRenderer);

		if (menuPage == MenuPage::Main)
		{
			mainMenuButtons.addTitle(pausedFromGameplay ? "Paused" : "Terraframe");

			if (mainMenuButtons.addButton("Resume Game"))
			{
				gameplayRunning = true;
				pausedFromGameplay = false;
			}

			if (mainMenuButtons.addButton("Load Last Save"))
			{
				if (loadWorld(gameplay.gameMap, gameplay.entityHolder, gameplay.player))
				{
					gameplayRunning = true;
					pausedFromGameplay = false;
				}
				else
				{
					Engine::traceLog(Engine::LogLevel::Warning, "Load failed: no valid save found.");
				}
			}

			if (mainMenuButtons.addButton("New Game"))
			{
				startNewGame();
			}

			if (mainMenuButtons.addButton("Settings"))
			{
				menuPage = MenuPage::Settings;
			}

			if (mainMenuButtons.addButton("Exit"))
			{
				return false;
			}
		}
		else
		{
			mainMenuButtons.addTitle("Settings");

			mainMenuButtons.addSlider("Master Volume", &getSettings().masterVolume, 0.f, 1.f);
			mainMenuButtons.addSlider("Music Volume", &getSettings().musicVolume, 0.f, 1.f);
			mainMenuButtons.addSlider("Sounds Volume", &getSettings().soundsVolume, 0.f, 1.f);

			if (mainMenuButtons.addButton("Reset To Defaults"))
			{
				getSettings() = Settings{};
				getSettings().sanitize();
			}

			if (mainMenuButtons.addButton("Back"))
			{
				menuPage = MenuPage::Main;
			}
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