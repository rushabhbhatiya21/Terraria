#include "drawBackground.h"
#include <window/window.h>
#include <assets/assetManager.h>
#include <assert.h>
#include <rendering/types/sprite.h>
#include <rendering/IRenderCollector.h>

void DrawBackground::draw(float deltaTime, Engine::AssetManager& assetManager, Engine::Cam& camera, Engine::Vec2 mapSize, Engine::Color4f skyColor, Engine::IRenderCollector& collector)
{
	auto drawBackground = [&](int type, float parallax, float opacity, Engine::Color4f skyColor)
		{
			const Engine::Texture* bg = &assetManager.forestBG;

			switch (type)
			{
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

			skyColor.a = opacity * 255;

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
				Engine::White,
				*bg,
				assetManager.defaultShader,
			};
			collector.submitSprite(bgSprite);
		};

	drawBackground(currentBackgroundType, 0.3f, 1.f, skyColor);

	// draw transition on top
	transitionTime -= deltaTime;
	if (transitionTime > 0)
	{
		float opacity = transitionTime;
		if (opacity > 1) { opacity = 1; }
		drawBackground(currentTransitionType, .3f, opacity, skyColor);
	}
}

void DrawBackground::setBackground(int background)
{
	if (background != currentBackgroundType)
	{
		if (transitionTime <= 0)
		{
			transitionTime = 1.f;
			currentTransitionType = currentBackgroundType;
			currentBackgroundType = background;
		}
	}
}