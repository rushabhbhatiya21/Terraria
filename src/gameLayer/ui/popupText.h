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
	bool crit = false;
};

extern std::vector<PopupText> popupTexts;

void DrawTextOutlined(
	Font font,
	const std::string& text,
	Vector2 position,
	float fontSize,
	float spacing,
	Color textColor,
	Color outlineColor,
	float thickness
);

void spawnPopupText(
	Vector2 position,
	Vector2 velocity,
	std::string text,
	float lifetime,
	float size,
	float offset,
	Color color,
	bool crit
);

void updatePopupText(
	float deltaTime
);

void drawPopuptext();