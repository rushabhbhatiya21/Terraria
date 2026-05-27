#pragma once
#include <raylib.h>

struct AssetManager;

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

	void draw(float deltaTime, AssetManager& assetManager, Camera2D camera, Vector2 mapSize, Color skyColor);

	void setBackground(int background);
};