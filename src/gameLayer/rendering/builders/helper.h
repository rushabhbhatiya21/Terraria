#pragma once
#include <array>
#include <math/vec2.h>
#include <math/rect.h>
#include <asserts.h>

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

inline std::array<Engine::Vec2, 4> generateTransformedCorners(
	const Engine::Rect& rect,
	const Engine::Vec2& origin,
	float rotation)
{
	auto corners = generateCorners(rect.width, rect.height);

	const float theta = rotation * Engine::Deg2Rad;
	const float c = cosf(theta);
	const float s = sinf(theta);

	for (auto& corner : corners)
	{
		corner -= origin;
		corner = rotateAroundOrigin(corner, c, s);
		corner += Engine::Vec2{ rect.x, rect.y };
	}

	return corners;
}

inline std::vector<Engine::Vec2> generateCirclePoints(const float radius, const int segments, const int start = 0, const int end = 0)
{
	permaAssert(segments >= 3);
	std::vector<Engine::Vec2> points;
	points.reserve(segments + 1);
	points.push_back(Engine::Vec2{ 0,0 });
	const float angleStep = (360.f / segments) * Engine::Deg2Rad;

	for (int i = start; i < segments; i++)
	{
		Engine::Vec2 circlePoint{};

		const float theta = i * angleStep;
		circlePoint.x = cosf(theta) * radius;
		circlePoint.y = sinf(theta) * radius;
		points.push_back(circlePoint);
	}

	return points;
}

inline std::vector<Engine::Vec2> generateTransformedCirclePoints(
	const float radius,
	const int segments,
	const float rotation,
	const Engine::Vec2 origin,
	const Engine::Vec2 position
)
{
	auto points = generateCirclePoints(radius, segments);

	const float theta = rotation * Engine::Deg2Rad;
	const float c = cosf(theta);
	const float s = sinf(theta);

	for (auto& point : points)
	{
		point -= origin;
		point = rotateAroundOrigin(point, c, s);
		point += position;
	}

	return points;
}

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