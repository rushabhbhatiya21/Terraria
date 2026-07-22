#pragma once
#include <vector>
#include <algorithm>
#include <rendering/IGeometrySink.h>
#include <rendering/types/index.h>
#include <rendering/types/renderState.h>
#include <rendering/builders/spriteGeometryBuilder.h>
#include <rendering/builders/rectGeometryBuilder.h>

namespace Engine
{
	struct Sprite;
	struct ColoredRect;
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

		SpriteGeometryBuilder& spriteBuilder;
		RectGeometryBuilder& rectBuilder;

	public:
		SpriteBatch(SpriteGeometryBuilder& spriteBuilder, RectGeometryBuilder& rectBuilder);
		void submitSprite(const Sprite& sprite);
		void submitRect(const ColoredRect& rect);
		void flush(IRenderBackend& backend);

	private:
		//std::vector<DrawCommand> buildDrawCommands();
		//void executeDrawCommands(const std::vector<DrawCommand>& commands);

		void beginEmission(const RenderState& renderState) override;
		void emitVertex(const Vertex& vertex) override;
		void emitIndex(const Index index) override;
		void endEmission() override;
		void sortByTextures();
	};
}