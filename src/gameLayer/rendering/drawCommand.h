#pragma once
#include <vector>
#include "vertex.h"

struct DrawCommand
{
	// geometry
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// render state
	Texture2D* texture;
};