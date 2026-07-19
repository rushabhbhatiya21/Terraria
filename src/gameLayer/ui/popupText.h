#pragma once
#include <string>
#include <vector>
#include <math/vec2.h>
#include <math/color.h>
#include <assets/font.h>

struct PopupText
{
	Engine::Vec2 position = {};
	Engine::Vec2 velocity = {};
	float startX = 0;

	std::string text;
	float lifetime = 0;
	float timer = 0;
	float size = 0;
	float baseSize = 0;

	Engine::Color4f color = Engine::White;
	bool crit = false;
};

extern std::vector<PopupText> popupTexts;

void DrawTextOutlined(
	const Engine::FontE& font,
	const std::string& text,
	Engine::Vec2 position,
	float fontSize,
	float spacing,
	Engine::Color4f textColor,
	Engine::Color4f outlineColor,
	float thickness
);

void spawnPopupText(
	Engine::Vec2 position,
	Engine::Vec2 velocity,
	std::string text,
	float lifetime,
	float size,
	float offset,
	Engine::Color4f color,
	bool crit
);

void updatePopupText(
	float deltaTime
);

void drawPopuptext();