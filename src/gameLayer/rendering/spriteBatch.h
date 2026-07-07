#pragma once
#include <vector>
#include "geometrySink.h"
#include "renderState.h"
#include "sprite.h"
#include "spriteGeometryBuilder.h"

struct DrawCommand;

class SpriteBatch : public IGeometrySink
{
	struct EmissionState
	{
		uint32_t firstVertex = 0;
		uint32_t firstIndex = 0;
		RenderState renderState = {};
	};

private:
	std::vector<Vertex>   vertexBuffer;
	std::vector<uint32_t> indexBuffer;
	std::vector<DrawCommand> drawCommands;

	EmissionState currentEmission;

	SpriteGeometryBuilder& builder;

public:
	SpriteBatch(SpriteGeometryBuilder& builder);
	void begin();
	void submitSprite(const Sprite& sprite);
	void end();

private:
	//std::vector<DrawCommand> buildDrawCommands();
	void executeDrawCommands(const std::vector<DrawCommand>& commands);
	void flush();

	void beginEmission(const RenderState& renderState) override;
	void emitVertex(const Vertex& vertex) override;
	void emitIndex(uint32_t index) override;
	void endEmission() override;
};