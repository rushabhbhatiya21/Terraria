#pragma once
#include <vector>
#include <string>
#include <raylib.h>

// top
Rectangle placeRectangleTopLeft(Rectangle r);
Rectangle placeRectangleCenterTop(Rectangle r, float w);
Rectangle placeReactangleTopRightCorner(Rectangle r, float w);

// center
Rectangle placeRectangleCenterLeft(Rectangle r, float h);
Rectangle placeRectangleCenter(Rectangle r, float w, float h);
Rectangle placeRectangleCenterRight(Rectangle r, float w, float h);

// bottom
Rectangle placeRectangleBottomLeftCorner(Rectangle r, float h);
Rectangle placeRectangleBottom(Rectangle r, float w, float h);
Rectangle placeRectangleBottomRightCorner(Rectangle r, float w, float h);


// implement 5 other functions to place in all possible spaces

Rectangle enlargeRectanglePixels(Rectangle r, float pixelsX, float pixelsY);

Rectangle shrinkRectanglePercentage(Rectangle r, float percentageX, float percentageY);

struct UIEngine
{
	enum Type
	{
		none,
		title,
		button
	};

	struct Widget
	{
		std::string text = "";
		int type = 0;

		bool isHovered = false;
		bool isBeingClicked = false;
		bool isReleased = false;
	};

	std::vector<Widget> widgets;

	std::vector<Widget> lastFrameWidgets;

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

	void updateAndRender();
};