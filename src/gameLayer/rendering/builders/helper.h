#pragma once
#include <array>
#include <math/vec2.h>

inline Engine::Vec2 rotateAroundOrigin(const Engine::Vec2& point, float c, float s)
{
	float xDash = (point.x * c) - (point.y * s);
	float yDash = (point.x * s) + (point.y * c);
	return Engine::Vec2{ xDash, yDash };
}

inline std::array<Engine::Vec2, 4> generateCorners(float w, float h)
{
	return std::array<Engine::Vec2, 4>
	{
		Engine::Vec2{ 0, 0 },
		Engine::Vec2{ w, 0 },
		Engine::Vec2{ w, h },
		Engine::Vec2{ 0, h }
	};
};

inline std::array<Engine::Vec2, 4> generateUVs(float x, float y, float w, float h)
{
	return std::array<Engine::Vec2, 4>
	{
		Engine::Vec2 { x    , y },
		Engine::Vec2 { x + w, y },
		Engine::Vec2 { x + w, y + h },
		Engine::Vec2 { x    , y + h }
	};
};

inline std::array<Engine::Vec2, 4> generateDefaultUVs()
{
	return std::array<Engine::Vec2, 4>
	{
		Engine::Vec2{ 0,0 },
		Engine::Vec2{ 1,0 },
		Engine::Vec2{ 1,1 },
		Engine::Vec2{ 0,1 }
	};
};