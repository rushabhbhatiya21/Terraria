#pragma once
#include <vector>
#include "vertex.h"
#include "renderState.h"

struct DrawCommand
{
	// geometry
	int firstVertex = 0;
	int vertexCount = 0;
	uint32_t firstIndex = 0;
	int indexCount = 0;
	//std::vector<Vertex> vertices;
	//std::vector<uint32_t> indices;

	// render state
	RenderState renderState = {};
};