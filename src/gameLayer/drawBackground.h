#pragma once
#include <math/vec2.h>
#include <math/cam.h>
#include <math/color.h>

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

	void draw(float deltaTime, Engine::AssetManager& assetManager, Engine::Cam& camera, Engine::Vec2 mapSize, Engine::Color4f skyColor, Engine::IRenderCollector& collector);

	void setBackground(int background);
};