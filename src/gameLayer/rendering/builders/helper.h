#pragma once
#include <array>
#include <algorithm>
#include <math/vec2.h>
#include <math/rect.h>
#include <asserts.h>
#include <rendering/types/meshPoint.h>

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

inline std::vector<Engine::MeshPoint> generateArcMesh(const float radius, const int segments, const int start = 0, int end = 0)
{
	permaAssert(segments >= 3);
	std::vector<Engine::MeshPoint> vertices;
	vertices.reserve(segments + 1);
	vertices.emplace_back(
		Engine::MeshPoint{
			Engine::Vec2{0,0},
			Engine::Vec2{0.5f, 0.5f}
		}
	);
	const float angleStep = (360.f / segments) * Engine::Deg2Rad;
	const float invDiameter = 1.f / (2.f * radius);

	if (start == 0 && end == 0)
		end = segments;

	for (int i = start; i < end; i++)
	{
		Engine::Vec2 circlePoint{};

		const float theta = i * angleStep;
		circlePoint.x = cosf(theta) * radius;
		circlePoint.y = sinf(theta) * radius;

		Engine::Vec2 uv{
			(circlePoint.x + (float)radius) / invDiameter,
			(circlePoint.y + (float)radius) / invDiameter,
		};

		vertices.emplace_back(
			Engine::MeshPoint
			{
				circlePoint,
				uv
			}
		);
	}

	return vertices;
}

inline std::vector<Engine::MeshPoint> generateTransformedArcMesh(
	const float radius,
	const int segments,
	const float rotation,
	const Engine::Vec2 origin,
	const Engine::Vec2 position,
	int start = 0,
	int end = 0
)
{
	auto vertices = generateArcMesh(radius, segments, start, end);

	const float theta = rotation * Engine::Deg2Rad;
	const float c = cosf(theta);
	const float s = sinf(theta);

	for (auto& vertex : vertices)
	{
		vertex.position -= origin;
		vertex.position = rotateAroundOrigin(vertex.position, c, s);
		vertex.position += position;
	}

	return vertices;
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

inline int calculateSegments(int radius)
{
	float c = 2.f * Engine::Pi * radius;
	float segments = std::ceilf(c / 4.f);
	segments = std::clamp(segments, 8.f, 64.f);
	return static_cast<int>(segments);
}