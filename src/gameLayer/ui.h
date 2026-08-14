#pragma once
#include <vector>
#include <string>
#include <math/rect.h>
#include <assets/font.h>

// top
Engine::Rect placeRectangleTopLeft(Engine::Rect r);
Engine::Rect placeRectangleCenterTop(Engine::Rect r, float w);
Engine::Rect placeReactangleTopRightCorner(Engine::Rect r, float w);

// center
Engine::Rect placeRectangleCenterLeft(Engine::Rect r, float h);
Engine::Rect placeRectangleCenter(Engine::Rect r, float w, float h);
Engine::Rect placeRectangleCenterRight(Engine::Rect r, float w, float h);

// bottom
Engine::Rect placeRectangleBottomLeftCorner(Engine::Rect r, float h);
Engine::Rect placeRectangleBottom(Engine::Rect r, float w, float h);
Engine::Rect placeRectangleBottomRightCorner(Engine::Rect r, float w, float h);


// implement 5 other functions to place in all possible spaces

Engine::Rect enlargeRectanglePixels(Engine::Rect r, float pixelsX, float pixelsY);

Engine::Rect enlargeRectanglePercentage(Engine::Rect r, float percentageX, float percentageY);
Engine::Rect shrinkRectanglePercentage(Engine::Rect r, float percentageX, float percentageY);

struct UIEngine
{
	enum Type
	{
		none,
		title,
		button,
		slider
	};

	struct Widget
	{
		std::string text = "";
		int type = 0;

		bool isHovered = false;
		bool isBeingClicked = false;
		bool isReleased = false;

		float* sliderValue = nullptr;
		float sliderMin = 0.f;
		float sliderMax = 1.f;
	};

	Engine::Font font;

	std::vector<Widget> widgets;

	std::vector<Widget> lastFrameWidgets;

	int activeSlider = -1;

	void init();

	bool addButton(std::string text)
	{
		Widget w;
		w.type = button;
		w.text = text;

		widgets.push_back(w);

		if (lastFrameWidgets.size() < widgets.size())
		{
			return false;
		}
		
		if (lastFrameWidgets[widgets.size() - 1].type != button)
		{
			return false;
		}

		return lastFrameWidgets[widgets.size() - 1].isReleased;
	}

	void addTitle(std::string text)
	{
		Widget w;
		w.type = title;
		w.text = text;

		widgets.push_back(w);
	}

	void addSlider(std::string text, float* value, float min = 0.f, float max = 1.f)
	{
		Widget w;
		w.type = slider;
		w.text = text;
		w.sliderValue = value;
		w.sliderMin = min;
		w.sliderMax = max;

		widgets.push_back(w);
	}

	void updateAndRender();
};