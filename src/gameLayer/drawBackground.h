#pragma once
#include <raylib.h>
//#include <rendering/IRenderCollector.h>

namespace Engine
{
	struct AssetManager;
	class SpriteBatch;
	class IRenderCollector;
}


struct DrawBackground
{
	enum BackgroundType
	{
		none = 0,
		night,
		forest,
		desert,
		snow,
		cave
	};

	int currentBackgroundType = forest;

	int currentTransitionType = 0;
	float transitionTime = 0.f;

	void draw(float deltaTime, Engine::AssetManager& assetManager, Camera2D camera, Vector2 mapSize, Color skyColor, Engine::IRenderCollector& collector);

	void setBackground(int background);
};