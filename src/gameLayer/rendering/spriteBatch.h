#pragma once
#include <vector>
#include "index.h"
#include "IGeometrySink.h"
#include "renderState.h"
#include "sprite.h"
#include "spriteGeometryBuilder.h"

struct DrawCommand;
class IRenderBackend;

class SpriteBatch : public IGeometrySink
{
	struct EmissionState
	{
		uint32_t firstVertex = 0;
		uint32_t firstIndex = 0;
		RenderState renderState = {};

		void clear()
		{
			uint32_t firstVertex = 0;
			uint32_t firstIndex = 0;
			RenderState renderState = {};
		}
	};

private:
	std::vector<Vertex>      vertexBuffer;
	std::vector<Index>       indexBuffer;
	std::vector<DrawCommand> drawCommands;

	EmissionState currentEmission;

	SpriteGeometryBuilder& builder;

public:
	SpriteBatch(SpriteGeometryBuilder& builder);
	void begin();
	void submitSprite(const Sprite& sprite);
	void end(IRenderBackend& backend);

private:
	//std::vector<DrawCommand> buildDrawCommands();
	//void executeDrawCommands(const std::vector<DrawCommand>& commands);
	void flush(IRenderBackend& backend) const;

	void beginEmission(const RenderState& renderState) override;
	void emitVertex(const Vertex& vertex) override;
	void emitIndex(const Index index) override;
	void endEmission() override;
};