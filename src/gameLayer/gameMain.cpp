#include "gameMain.h"
#include <gameplay.h>
#include <assetManager.h>
#include <audio.h>
#include <settings.h>

#pragma region global variables

AssetManager assetManager;
Gameplay gameplay;

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
	return gameplay.update(assetManager);
}

#pragma region close game logic

void closeGame()
{
	//UnloadRenderTexture(lightMask);
	printf("\n\nCLOSED!!!!!!!!!\n\n");
}

#pragma endregion