#include "popupText.h"
#include <algorithm>
#include <cmath>
#include <assets/assetManager.h>
#include <rendering/IRenderCollector.h>
#include <rendering/types/text.h>
#include <ui/textLayout.h>

std::vector<PopupText> popupTexts;
//static std::string critText = "CRITICAL";

void spawnPopupText(Engine::Vec2 position,
	Engine::Vec2 velocity,
	std::string text,
	float lifetime,
	float size,
	float offset,
	Engine::Color4f color,
	bool crit
)
{
	PopupText popup{};

	popup.position = position;
	popup.startX = position.x;

	popup.position.y += crit ? (offset * 2.f) : offset;

	popup.velocity = velocity;
	popup.text = text;
	popup.lifetime = lifetime;
	popup.timer = lifetime;
	popup.baseSize = size;
	popup.color = color;
	popup.crit = crit;

	popupTexts.push_back(popup);
}

void updatePopupText(float deltaTime)
{
	for (int i = (int)popupTexts.size() - 1; i >= 0; --i)
	{
		PopupText& popup = popupTexts[i];

		popup.timer -= deltaTime;

		if (popup.timer <= 0.f)
		{
			popupTexts.erase(popupTexts.begin() + i);
			continue;
		}

		float progress = 1.f - (popup.timer / popup.lifetime);

		//// horizontal wavy motion
		//popup.position.x = popup.startX + sin(progress * PI * 2.f) * 0.2f;

		// vertical upward movement
		popup.position.y -= (popup.velocity.y * deltaTime);

		// vertical velocity
		popup.velocity.y += .01f;

		// fade
		float fade = 1.f - pow(progress, 3.f); // strongly visible at start and then fade away fast in the end
		popup.color.a = (unsigned char)(255.f * fade);

		// size 
		popup.size = popup.baseSize * (1.2f - progress * 0.8f) + sin(progress * Engine::Pi) * 0.2f;


		if (popup.size < 0.f)
			popup.size = 0.f;
	}
}

void DrawTextOutlined(
	const Engine::Font& font,
	const std::string& text,
	Engine::Vec2 position,
	float fontSize,
	float spacing,
	Engine::Color4f textColor,
	Engine::Color4f outlineColor,
	float thickness
)
{
	// outline
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 && y == 0)
				continue;

			//DrawTextEx(
			//	font,
			//	text.c_str(),
			//	{
			//		position.x + x * thickness,
			//		position.y + y * thickness
			//	},
			//	fontSize,
			//	spacing,
			//	outlineColor
			//);
		}
	}

	//// main text
	//DrawTextEx(
	//	font,
	//	text.c_str(),
	//	position,
	//	fontSize,
	//	spacing,
	//	textColor
	//);
}

void drawPopuptext(const Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	for (int i = (int)popupTexts.size() - 1; i >= 0; --i)
	{
		PopupText& popup = popupTexts[i];

		float size = popup.crit ? popup.size * 2.f : popup.size;
		Engine::Color4f mainColor = popup.crit ? Engine::Color4f{ 255, 80, 80, popup.color.a } : popup.color;

		Engine::Text shadow
		{
			{ popup.position.x - size * 0.06f, popup.position.y + size * 0.06f },
			Engine::TextLayout::Anchor::TopLeft,
			0.f,
			Engine::Color4f{ 0, 0, 0, (unsigned char)std::min(200, (int)popup.color.a) },
			popup.text,
			size,
			0.02f,
			&assetManager.defaultFont,
			&assetManager.defaultShader
		};

		Engine::Text main
		{
			popup.position,
			Engine::TextLayout::Anchor::TopLeft,
			0.f,
			mainColor,
			popup.text,
			size,
			0.02f,
			&assetManager.defaultFont,
			&assetManager.defaultShader
		};

		collector.submitText(shadow);
		collector.submitText(main);
	}
}
