#include "popupText.h"
#include <raymath.h>

std::vector<PopupText> popupTexts;
//static std::string critText = "CRITICAL";

void spawnPopupText(Vector2 position, Vector2 velocity, std::string text, float lifetime, float size, float offset, Color color, bool crit)
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
		popup.size = popup.baseSize * (1.2f - progress * 0.8f) + sin(progress * PI) * 0.2f;


		if (popup.size < 0.f)
			popup.size = 0.f;
	}
}

void DrawTextOutlined(
	Font font,
	const std::string& text,
	Vector2 position,
	float fontSize,
	float spacing,
	Color textColor,
	Color outlineColor,
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

			DrawTextEx(
				font,
				text.c_str(),
				{
					position.x + x * thickness,
					position.y + y * thickness
				},
				fontSize,
				spacing,
				outlineColor
			);
		}
	}

	// main text
	DrawTextEx(
		font,
		text.c_str(),
		position,
		fontSize,
		spacing,
		textColor
	);
}

void drawPopuptext()
{
	for (int i = (int)popupTexts.size() - 1; i >= 0; --i)
	{
		PopupText& popup = popupTexts[i];
		if (popup.crit)
		{
			//Vector2 critSize = MeasureTextEx(
			//	GetFontDefault(),
			//	critText.c_str(),
			//	popup.size,
			//	0.02f
			//);

			//Vector2 damageSize = MeasureTextEx(
			//	GetFontDefault(),
			//	popup.text.c_str(),
			//	popup.size,
			//	0.02f
			//);

			//float maxWidth = std::max(critSize.x, damageSize.x);

			//float critX = popup.position.x + (maxWidth - critSize.x) / 2.f;

			//float damageX = popup.position.x + (maxWidth - damageSize.x) / 2.f;

			//DrawTextOutlined(
			//	GetFontDefault(), 
			//	critText,
			//	{ critX, popup.position.y },
			//	popup.size,
			//	0.02f,
			//	Color{ 255, 80, 80, 255 },
			//	BLACK,
			//	.05f
			//);

			DrawTextOutlined(
				GetFontDefault(),
				popup.text.c_str(),
				popup.position,
				popup.size * 2,
				0.05f,
				Color{ 255, 80, 80, 255 },
				Color{ 0,0,0,160 },
				.03f
			);
		}
		else
		{
			DrawTextOutlined(
				GetFontDefault(),
				popup.text.c_str(),
				popup.position,
				popup.size,
				0.05f,
				popup.color,
				Color{ 0,0,0,160 },
				.03f
			);
		}
		//DrawTextEx(
		//	GetFontDefault(),
		//	popup.text.c_str(),
		//	popup.position,
		//	popup.size,
		//	.02f,
		//	popup.color
		//);
	}
}
