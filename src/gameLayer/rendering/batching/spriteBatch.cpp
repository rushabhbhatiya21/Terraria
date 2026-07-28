#include "spriteBatch.h"
#include <asserts.h>
#include <rendering/IRenderBackend.h>

namespace Engine
{
	SpriteBatch::SpriteBatch()
	{
	}

	void SpriteBatch::submitSprite(const Sprite& sprite)
	{
		spriteBuilder.build(sprite, *this);
	}

	void SpriteBatch::submitRect(const ColoredRect& rect)
	{
		rectBuilder.build(rect, *this);
	}

	void SpriteBatch::submitLine(const Line& line)
	{
		lineBuilder.build(line, *this);
	}

	void SpriteBatch::submitOutlinedRect(const OutlinedRect& rect)
	{
		outlinedRectBuilder.build(rect, *this);
	}

	void SpriteBatch::submitCircle(const Circle& circle)
	{
		circleBuilder.build(circle, *this);
	}

	void SpriteBatch::submitOutlinedCircle(const OutlinedCircle& circle)
	{
		outlinedCircleBuilder.build(circle, *this);
	}

	void SpriteBatch::submitArc(const Arc& arc)
	{
		arcBuilder.build(arc, *this);
	}


	void SpriteBatch::submitRoundedRect(const RoundedRect& roundedRect)
	{
		roundedRectBuilder.build(roundedRect, *this);
	}

	void SpriteBatch::flush(IRenderBackend& backend)
	{
		sortByTextures();
		backend.render(vertexBuffer, indexBuffer, drawCommands);

		vertexBuffer.clear();
		indexBuffer.clear();
		drawCommands.clear();
	}

	void SpriteBatch::beginEmission(const RenderState& renderState)
	{
		currentEmission.clear();
		currentEmission.firstVertex = (uint32_t)vertexBuffer.size();
		currentEmission.firstIndex = (uint32_t)indexBuffer.size();
		currentEmission.renderState = renderState;
	}

	void SpriteBatch::emitVertex(const Vertex& vertex)
	{
		vertexBuffer.emplace_back(vertex);
	}

	void SpriteBatch::emitIndex(const Index index)
	{
		uint32_t finalIndex = currentEmission.firstVertex + static_cast<uint32_t>(index);
		permaAssertCommentDevelopement(finalIndex <= 65535, "SpriteBatch index overflow! Exceeded 65535 vertices.");
		indexBuffer.push_back(static_cast<Index>(finalIndex));
	}

	void SpriteBatch::endEmission()
	{
		DrawCommand drawCommand
		{
			currentEmission.firstVertex,
			(uint32_t)vertexBuffer.size() - currentEmission.firstVertex,
			currentEmission.firstIndex,
			(uint32_t)indexBuffer.size() - currentEmission.firstIndex,
			currentEmission.renderState
		};

		drawCommands.emplace_back(drawCommand);
	}

	void SpriteBatch::sortByTextures()
	{
		std::sort(drawCommands.begin(), drawCommands.end(),
			[](const DrawCommand& a, const DrawCommand& b)
			{
				return a.renderState.texture <
					b.renderState.texture;
			});
	}

}