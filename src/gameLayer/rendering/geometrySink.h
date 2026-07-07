#pragma once

struct Vertex;
class RenderState;

class IGeometrySink
{
public:
	virtual void beginEmission(const RenderState& renderState);
	virtual void emitVertex(const Vertex& vertex);
	virtual void emitIndex(const uint32_t index);
	virtual void endEmission();

	virtual ~IGeometrySink() = default;
};