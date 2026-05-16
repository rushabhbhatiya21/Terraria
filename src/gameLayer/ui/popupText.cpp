#include "popupText.h"
#include <raymath.h>

std::vector<PopupText> popupTexts;

void spawnPopupText(Vector2 position, Vector2 velocity, std::string text, float lifetime, float size, float offset, Color color)
{
	PopupText popup{};

	popup.position = position;
	popup.startX = position.x;
	popup.position.y += offset;

	popup.velocity = velocity;
	popup.text = text;
	popup.lifetime = lifetime;
	popup.timer = lifetime;
	popup.baseSize = size;
	popup.color = color;

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

void drawPopuptext()
{
	for (int i = (int)popupTexts.size() - 1; i >= 0; --i)
	{
		PopupText& popup = popupTexts[i];
		DrawTextEx(
			GetFontDefault(),
			popup.text.c_str(),
			popup.position,
			popup.size,
			.02f,
			popup.color
		);
	}
}
