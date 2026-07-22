#include "spriteBatch.h"
#include "spriteBatch.h"
#include "spriteBatch.h"
#include <asserts.h>
#include <rendering/types/sprite.h>
#include <rendering/types/coloredRect.h>
#include <rendering/types/line.h>
#include <rendering/types/outlinedRect.h>
#include <rendering/batching/drawCommand.h>
#include <rendering/IRenderBackend.h>

namespace Engine
{
	SpriteBatch::SpriteBatch(
		SpriteGeometryBuilder& sb,
		RectGeometryBuilder& rb,
		LineGeometryBuilder& lb,
		OutlinedRectGeometryBuilder& orb
	)
		: spriteBuilder(sb), rectBuilder(rb), lineBuilder(lb), outlinedRectBuilder(orb)
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


	//std::vector<DrawCommand> SpriteBatch::buildDrawCommands()
	//{
	//	std::vector<DrawCommand> commands;
	//	commands.reserve(sprites.size());
	//
	//	for (const auto& sprite : sprites)
	//	{
	//		const DrawCommand command = builder.build(sprite);
	//		commands.push_back(command);
	//	}
	//	return commands;
	//
	//}

	//void SpriteBatch::executeDrawCommands(const std::vector<DrawCommand>& commands)
	//{
		//for (const auto& command : commands)
		//{
		//	const Sprite& sprite = command.sprite;
		//	DrawTexturePro(
		//		sprite.texture,  // texture
		//		sprite.srcRect,  // source
		//		sprite.destRect, // dest
		//		sprite.origin,   // origin (top-left)
		//		sprite.rotation, // rotation
		//		sprite.tint
		//	);
		//}
	//}

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