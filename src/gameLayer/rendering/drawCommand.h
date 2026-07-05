#pragma once
#include <array>
#include "vertex.h"

struct DrawCommand
{
	// geometry
	std::array<Vertex, 4> vertices;

	// render state
	Texture2D* texture;
};