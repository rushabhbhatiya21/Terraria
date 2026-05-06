#pragma once
#include <raylib.h>

Rectangle placeReactangleTopRightCorner(Rectangle r, float w);

Rectangle placeRectangleCenter(Rectangle r, float w, float h);

Rectangle placeRectangleTopLeft(Rectangle r);

// implement 8 other functions to place in all possible spaces

Rectangle enlargeRectanglePixels(Rectangle r, float pixelsX, float pixelsY);

Rectangle shrinkRectanglePercentage(Rectangle r, float percentageX, float percentageY);