#pragma once
#include "index.h"

namespace Engine
{
	struct Vertex;
	struct RenderState;

	class IGeometrySink
	{
	public:
		virtual void beginEmission(const RenderState& renderState) = 0;
		virtual void emitVertex(const Vertex& vertex) = 0;
		virtual void emitIndex(const Index index) = 0;
		virtual void endEmission() = 0;

		virtual ~IGeometrySink() = default;
	};
}