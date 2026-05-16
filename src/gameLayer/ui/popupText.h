#pragma once
#include <raylib.h>
#include <string>
#include <vector>

struct PopupText
{
	Vector2 position = {};
	Vector2 velocity = {};
	float startX = 0;

	std::string text;
	float lifetime = 0;
	float timer = 0;
	float size = 0;
	float baseSize = 0;

	Color color = WHITE;
};

extern std::vector<PopupText> popupTexts;

void spawnPopupText(
	Vector2 position,
	Vector2 velocity,
	std::string text,
	float lifetime,
	float size,
	float offset,
	Color color
);

void updatePopupText(
	float deltaTime
);

void drawPopuptext();