#include "drawBackground.h"
#include <window/window.h>
#include <assets/assetManager.h>
#include <assert.h>
#include <rendering/types/sprite.h>
#include <rendering/IRenderCollector.h>

void DrawBackground::draw(float deltaTime, Engine::AssetManager& assetManager, Engine::Cam& camera, Engine::Vec2 mapSize, Engine::Color4f skyColor, Engine::IRenderCollector& collector)
{
	auto drawBackground = [&](int type, float parallax, float opacity, Engine::Color4f skyColor, bool keepVisible)
		{
			const Engine::Texture* bg = &assetManager.forestBG;

			switch (type)
			{
				case night:  bg = &assetManager.nightBG;  break;
				case forest: bg = &assetManager.forestBG; break;
				case desert: bg = &assetManager.desertBG; break;
				case snow:   bg = &assetManager.snowBG; break;
				case cave:   bg = &assetManager.caveBG; break;
					
				default:
					break;
			}

			assert(bg);

			int screenW = Engine::getScreenWidth();
			int screenH = Engine::getScreenHeight();

			float aspectRatio = (float)bg->getWidth()/ (float)bg->getHeight();
			float bgScaleScreen = 2.f;

			float base = std::max(screenW, screenH) * bgScaleScreen;

			float bgW = base;
			float bhH = base;

			if (aspectRatio > 1.f)
			{
				// scale height
				bhH = bgW / aspectRatio;
			}
			else
			{
				// scale width
				bgW = bhH / aspectRatio;
			}

			// compute camera ranges
			const float halfViewW = (screenW * .5f) / camera.zoom;
			const float halfViewH = (screenH * .5f) / camera.zoom;

			const float cameraMinX = halfViewW;
			const float cameraMaxX = mapSize.x - halfViewW;
			const float cameraMinY = halfViewH;
			const float cameraMaxY = mapSize.y - halfViewH;

			Engine::Vec2 cameraPos = camera.target;
			cameraPos.x = std::clamp(cameraPos.x, cameraMinX, cameraMaxX);
			cameraPos.y = std::clamp(cameraPos.y, cameraMinY, cameraMaxY);

			const float cameraRangeX = std::max(0.f, cameraMaxX - cameraMinX);
			const float cameraRangeY = std::max(0.f, cameraMaxY - cameraMinY);

			const float normX = cameraRangeX > 0.f ? (cameraPos.x - cameraMinX) / cameraRangeX : 0.f;
			const float normY = cameraRangeY > 0.f ? (cameraPos.y - cameraMinY) / cameraRangeY : 0.f;

			const float maxOffX = bgW - screenW;
			const float maxOffY = bhH - screenH;

			const float offX = -maxOffX * normX * parallax;
			const float offY = -maxOffY * normY * parallax;

			Engine::Rect src = { 0,0,(float)bg->getWidth(),(float)bg->getHeight()};
			Engine::Rect dest = { offX, offY, bgW, bhH };

			const float tintStrength = 0.18f;
			Engine::Color4f tintColor = {};
			tintColor.r = (unsigned char)std::clamp((255.f * (1.f - tintStrength)) + (skyColor.r * tintStrength), 0.f, 255.f);
			tintColor.g = (unsigned char)std::clamp((255.f * (1.f - tintStrength)) + (skyColor.g * tintStrength), 0.f, 255.f);
			tintColor.b = (unsigned char)std::clamp((255.f * (1.f - tintStrength)) + (skyColor.b * tintStrength), 0.f, 255.f);

			float alpha01 = std::clamp(opacity, 0.f, 1.f);
			if (keepVisible)
			{
				alpha01 = std::max(alpha01, 0.88f);
			}
			tintColor.a = (unsigned char)std::clamp(alpha01 * 255.f, 0.f, 255.f);

			//DrawTexturePro(
			//	bg, 
			//	src, 
			//	dest, 
			//	{ 0,0 }, 
			//	0.f, 
			//	skyColor
			//);

			Engine::Sprite bgSprite
			{
				src,
				dest,
				{ 0,0 },
				0.f,
				tintColor,
				*bg,
				assetManager.defaultShader,
			};
			collector.submitSprite(bgSprite);
		};

	float remaining = 0.f;
	if (transitionDuration > 0.f)
	{
		remaining = std::clamp(transitionTime / transitionDuration, 0.f, 1.f);
	}

	if (transitionTime > 0.f)
	{
		// Use reciprocal fade curves so transition is visible even if sorted draw order flips.
		float oldOpacity = remaining * remaining * (3.f - 2.f * remaining); // smoothstep(remaining)
		float progress = 1.f - remaining;
		float newOpacity = progress * progress * (3.f - 2.f * progress); // smoothstep(progress)

		drawBackground(currentTransitionType, 0.34f, oldOpacity, skyColor, false);
		drawBackground(currentBackgroundType, 0.30f, newOpacity, skyColor, false);

		transitionTime -= deltaTime;
		if (transitionTime < 0.f) { transitionTime = 0.f; }
	}
	else
	{
		drawBackground(currentBackgroundType, 0.30f, 1.f, skyColor, true);
	}
}

void DrawBackground::setBackground(int background)
{
	if (background != currentBackgroundType)
	{
		if (transitionTime <= 0)
		{
			transitionTime = transitionDuration;
			currentTransitionType = currentBackgroundType;
			currentBackgroundType = background;
		}
	}
}