#include "drawBackground.h"
#include <assetManager.h>
#include <raymath.h>

void DrawBackground::draw(float deltaTime, AssetManager& assetManager, Camera2D camera, Vector2 mapSize, Color skyColor)
{
	auto drawBackground = [&](int type, float parallax, float opacity, Color skyColor)
		{
			Texture bg = assetManager.forestBG;

			switch (type)
			{
				case forest: bg = assetManager.forestBG; break;
				case desert: bg = assetManager.desertBG; break;
				case snow: bg = assetManager.snowBG; break;
				case cave: bg = assetManager.caveBG; break;
					
				default:
					break;
			}

			int screenW = GetScreenWidth();
			int screenH = GetScreenHeight();

			float aspectRatio = (float)bg.width / (float)bg.height;
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

			Vector2 cameraPos = camera.target;
			cameraPos.x = Clamp(cameraPos.x, cameraMinX, cameraMaxX);
			cameraPos.y = Clamp(cameraPos.y, cameraMinY, cameraMaxY);

			const float cameraRangeX = std::max(0.f, cameraMaxX - cameraMinX);
			const float cameraRangeY = std::max(0.f, cameraMaxY - cameraMinY);

			const float normX = cameraRangeX > 0.f ? (cameraPos.x - cameraMinX) / cameraRangeX : 0.f;
			const float normY = cameraRangeY > 0.f ? (cameraPos.y - cameraMinY) / cameraRangeY : 0.f;

			const float maxOffX = bgW - screenW;
			const float maxOffY = bhH - screenH;

			const float offX = -maxOffX * normX * parallax;
			const float offY = -maxOffY * normY * parallax;

			Rectangle src = { 0,0,(float)bg.width,(float)bg.height };
			Rectangle dest = { offX, offY, bgW, bhH };

			skyColor.a = opacity * 255;

			DrawTexturePro(
				bg, 
				src, 
				dest, 
				{ 0,0 }, 
				0.f, 
				skyColor
			);
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