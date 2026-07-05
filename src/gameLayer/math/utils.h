#pragma once
#include <raylib.h>

Vector2 rotateAroundOrigin(const Vector2& point, float c, float s)
{
	float xDash = (point.x * c) - (point.y * s);
	float yDash = (point.x * s) + (point.y * c);
	return Vector2{ xDash, yDash };
}