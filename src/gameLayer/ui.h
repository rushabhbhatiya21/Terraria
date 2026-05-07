#pragma once
#include <vector>
#include <string>
#include <raylib.h>

Rectangle placeRectangleTopLeft(Rectangle r);

Rectangle placeRectangleCenterTop(Rectangle r, float w);

Rectangle placeReactangleTopRightCorner(Rectangle r, float w);

Rectangle placeRectangleCenter(Rectangle r, float w, float h);

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

	bool addButton(std::string text)
	{
		Widget w;
		w.type = button;
		w.text = text;

		widgets.push_back(w);
		return false;
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