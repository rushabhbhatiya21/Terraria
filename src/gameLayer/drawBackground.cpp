#include "drawBackground.h"
#include <window/window.h>
#include <assets/assetManager.h>
#include <assert.h>
#include <algorithm>
#include <rendering/types/sprite.h>
#include <rendering/IRenderCollector.h>

void DrawBackground::draw(float deltaTime, Engine::AssetManager& assetManager, Engine::Cam& camera, Engine::Vec2 mapSize, Engine::Color4f skyColor, Engine::IRenderCollector& collector)
{
	auto makeTint = [&](Engine::Color4f skyColor, float tintStrength, float opacity)
		{
			Engine::Color4f tintColor = {};
			tintColor.r = (unsigned char)std::clamp((255.f * (1.f - tintStrength)) + (skyColor.r * tintStrength), 0.f, 255.f);
			tintColor.g = (unsigned char)std::clamp((255.f * (1.f - tintStrength)) + (skyColor.g * tintStrength), 0.f, 255.f);
			tintColor.b = (unsigned char)std::clamp((255.f * (1.f - tintStrength)) + (skyColor.b * tintStrength), 0.f, 255.f);
			tintColor.a = (unsigned char)std::clamp(std::clamp(opacity, 0.f, 1.f) * 255.f, 0.f, 255.f);
			return tintColor;
		};

	auto drawLayer = [&](const Engine::Texture& tex, float parallaxX, float parallaxY, float opacity, Engine::Color4f skyColor, float tintStrength, bool followCameraX, bool followCameraY)
		{
			if (tex.getWidth() <= 0 || tex.getHeight() <= 0) { return; }

			int screenW = Engine::getScreenWidth();
			int screenH = Engine::getScreenHeight();

			float aspectRatio = (float)tex.getWidth()/ (float)tex.getHeight();
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

			float offX = 0.f;
			float offY = 0.f;

			if (followCameraX)
			{
				offX = -maxOffX * normX * parallaxX;
			}

			if (followCameraY)
			{
				offY = -maxOffY * normY * parallaxY;
			}

			Engine::Rect src = { 0,0,(float)tex.getWidth(),(float)tex.getHeight()};
			Engine::Rect dest = { offX, offY, bgW, bhH };

			Engine::Color4f tintColor = makeTint(skyColor, tintStrength, opacity);

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
				tex,
				assetManager.defaultShader,
			};
			collector.submitSprite(bgSprite);
		};

	auto drawLayerTopSingle = [&](const Engine::Texture& tex, float opacity, Engine::Color4f skyColor, float tintStrength, float scale, Engine::Rect* outDest = nullptr)
		{
			if (tex.getWidth() <= 0 || tex.getHeight() <= 0) { return; }

			int screenW = Engine::getScreenWidth();

			float tileW = std::max(1.f, tex.getWidth() * scale);
			float tileH = std::max(1.f, tex.getHeight() * scale);
			float x = (screenW - tileW) * 0.5f;
			float y = 0.f;

			Engine::Color4f tintColor = makeTint(skyColor, tintStrength, opacity);
			Engine::Rect src = { 0,0,(float)tex.getWidth(),(float)tex.getHeight() };

			Engine::Sprite bgSprite
			{
				src,
				{ x, y, tileW, tileH },
				{ 0,0 },
				0.f,
				tintColor,
				tex,
				assetManager.defaultShader,
			};
			collector.submitSprite(bgSprite);

			if (outDest)
			{
				*outDest = { x, y, tileW, tileH };
			}
		};

	auto drawLayerInRect = [&](const Engine::Texture& tex, const Engine::Rect& dest, float opacity, Engine::Color4f skyColor, float tintStrength)
		{
			if (tex.getWidth() <= 0 || tex.getHeight() <= 0) { return; }

			Engine::Color4f tintColor = makeTint(skyColor, tintStrength, opacity);
			Engine::Rect src = { 0,0,(float)tex.getWidth(),(float)tex.getHeight() };

			Engine::Sprite bgSprite
			{
				src,
				dest,
				{ 0,0 },
				0.f,
				tintColor,
				tex,
				assetManager.defaultShader,
			};
			collector.submitSprite(bgSprite);
		};

	auto drawBackgroundStack = [&](int type, float opacity01, Engine::Color4f skyColor, bool keepVisible)
		{
			float a = std::clamp(opacity01, 0.f, 1.f);
			if (keepVisible)
			{
				a = std::max(a, 0.88f);
			}

			switch (type)
			{
			case forest:
				drawLayer(assetManager.forestBG, 0.08f, 0.02f, a, skyColor, 0.12f, true, true);                    // back
				drawLayer(assetManager.forestMountainsFar, 0.16f, 0.03f, a, skyColor, 0.14f, true, true);          // back
				drawLayer(assetManager.forestMountainsClose, 0.32f, 0.05f, a, skyColor, 0.16f, true, true);        // mid
				drawLayer(assetManager.forestTrees, 0.56f, 0.08f, a, skyColor, 0.18f, true, true);                 // front
				drawLayer(assetManager.skySun, 0.03f, 0.00f, std::min(1.f, a * 0.95f), skyColor, 0.08f, true, false);
				break;

			case desert:
				drawLayer(assetManager.desertBG, 0.08f, 0.02f, a, skyColor, 0.12f, true, true);                    // back
				drawLayer(assetManager.desertDunesBack, 0.30f, 0.05f, a, skyColor, 0.15f, true, true);             // mid
				drawLayer(assetManager.desertDunesFront, 0.54f, 0.08f, a, skyColor, 0.18f, true, true);            // front
				drawLayer(assetManager.skySun, 0.03f, 0.00f, std::min(1.f, a * 0.95f), skyColor, 0.08f, true, false);
				break;

			case snow:
				drawLayer(assetManager.snowBG, 0.08f, 0.02f, a, skyColor, 0.12f, true, true);                      // back
				drawLayer(assetManager.snowMountainsBack, 0.18f, 0.03f, a, skyColor, 0.14f, true, true);           // back
				drawLayer(assetManager.snowMountainsFront, 0.32f, 0.05f, a, skyColor, 0.16f, true, true);          // mid
				drawLayer(assetManager.snowTrees, 0.58f, 0.08f, a, skyColor, 0.18f, true, true);                   // front
				drawLayer(assetManager.skySun, 0.03f, 0.00f, std::min(1.f, a * 0.95f), skyColor, 0.08f, true, false);
				break;

			case cave:
				drawLayer(assetManager.caveBG, 0.18f, 0.04f, a, skyColor, 0.12f, true, true);
				break;

			case night:
			{
				drawLayer(assetManager.nightBG, 0.08f, 0.02f, a, skyColor, 0.08f, true, true);
				Engine::Rect skyRect{};
				drawLayerTopSingle(assetManager.skyStars, std::min(1.f, a * 0.95f), skyColor, 0.03f, 2.6f, &skyRect);
				drawLayerInRect(assetManager.skyMoon, skyRect, std::min(1.f, a), skyColor, 0.0f);
				break;
			}

			default:
				drawLayer(assetManager.forestBG, 0.08f, 0.02f, a, skyColor, 0.12f, true, true);
				break;
			}
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

		drawBackgroundStack(currentTransitionType, oldOpacity, skyColor, false);
		drawBackgroundStack(currentBackgroundType, newOpacity, skyColor, false);

		transitionTime -= deltaTime;
		if (transitionTime < 0.f) { transitionTime = 0.f; }
	}
	else
	{
		drawBackgroundStack(currentBackgroundType, 1.f, skyColor, true);
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