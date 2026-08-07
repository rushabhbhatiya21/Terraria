#pragma once
#include <vector>
#include <algorithm>
#include <rendering/IGeometrySink.h>
#include <rendering/types/index.h>
#include <rendering/types/renderState.h>

// builders
#include <rendering/builders/spriteGeometryBuilder.h>
#include <rendering/builders/rectGeometryBuilder.h>
#include <rendering/builders/lineGeometryBuilder.h>
#include <rendering/builders/outlinedRectGeometryBuilder.h>
#include <rendering/builders/circleGeometryBuilder.h>
#include <rendering/builders/outlinedCircleGeometryBuilder.h>
#include <rendering/builders/arcGeometryBuilder.h>
#include <rendering/builders/roundedRectGeometryBuilder.h>
#include <rendering/builders/textGeometryBuilder.h>

namespace Engine
{
	struct Sprite;
	struct ColoredRect;
	struct Line;
	struct OutlinedRect;
	struct Circle;
	struct OutlinedCircle;
	struct Arc;
	struct RoundedRect;
	struct Text;

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

		SpriteGeometryBuilder spriteBuilder;
		RectGeometryBuilder rectBuilder;
		LineGeometryBuilder lineBuilder;
		OutlinedRectGeometryBuilder outlinedRectBuilder{ lineBuilder };
		CircleGeometryBuilder circleBuilder;
		OutlinedCircleGeometryBuilder outlinedCircleBuilder{ lineBuilder };
		ArcGeometryBuilder arcBuilder;
		RoundedRectGeometryBuilder roundedRectBuilder{ rectBuilder, arcBuilder };
		TextGeometryBuilder textBuilder{ spriteBuilder };

	public:
		SpriteBatch();
		void submitSprite(const Sprite& sprite);
		void submitRect(const ColoredRect& rect);
		void submitLine(const Line& rect);
		void submitOutlinedRect(const OutlinedRect& rect);
		void submitCircle(const Circle& circle);
		void submitOutlinedCircle(const OutlinedCircle& circle);
		void submitArc(const Arc& arc);
		void submitRoundedRect(const RoundedRect& roundedRect);
		void submitText(const Text& text);

		void flush(IRenderBackend& backend);

	private:
		void beginEmission(const RenderState& renderState) override;
		void emitVertex(const Vertex& vertex) override;
		void emitIndex(const Index index) override;
		void endEmission() override;
		void sortByTextures();
	};
}