#include "spriteBatch.h"
#include "drawCommand.h"

SpriteBatch::SpriteBatch(SpriteGeometryBuilder& builder)
	: builder(builder)
{
}

void SpriteBatch::begin()
{
	vertexBuffer.clear();
	indexBuffer.clear();
	drawCommands.clear();
}

void SpriteBatch::submitSprite(const Sprite& sprite)
{
	builder.build(sprite, *this);
}

void SpriteBatch::end()
{
	flush();
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

void SpriteBatch::flush()
{
	//std::vector<DrawCommand> commands = buildDrawCommands();
	//executeDrawCommands(drawCommands);
}

void SpriteBatch::beginEmission(const RenderState& renderState)
{
	currentEmission.clear();
	currentEmission.firstVertex = (uint32_t)vertexBuffer.size();
	currentEmission.firstIndex  = (uint32_t)indexBuffer.size();
	currentEmission.renderState = renderState;
}

void SpriteBatch::emitVertex(const Vertex& vertex)
{
	vertexBuffer.emplace_back(vertex);
}

void SpriteBatch::emitIndex(uint32_t index)
{
	indexBuffer.emplace_back(currentEmission.firstVertex + index);
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
