#pragma once
#include <array>
#include <vector>
#include <math/vec2.h>
#include <math/rect.h>
#include <rendering/types/meshPoint.h>

Engine::Vec2 rotateAroundOrigin(const Engine::Vec2& point, float c, float s);

std::array<Engine::Vec2, 4> generateCorners(float w, float h);

std::array<Engine::Vec2, 4> generateTransformedCorners(
	const Engine::Rect& rect,
	const Engine::Vec2& origin,
	float rotation);

std::vector<Engine::MeshPoint> generateArcMesh(const float radius, const int segments, const int start = 0, int end = 0);

std::vector<Engine::MeshPoint> generateTransformedArcMesh(
	const float radius,
	const int segments,
	const float rotation,
	const Engine::Vec2 origin,
	const Engine::Vec2 position,
	int start = 0,
	int end = 0
);

std::array<Engine::Vec2, 4> generateUVs(float x, float y, float w, float h);

std::array<Engine::Vec2, 4> generateDefaultUVs();

int calculateSegments(int radius);