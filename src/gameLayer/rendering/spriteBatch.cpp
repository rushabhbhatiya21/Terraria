#include "spriteBatch.h"
#include "drawCommand.h"

SpriteBatch::SpriteBatch(SpriteGeometryBuilder& builder)
	: builder(builder)
{
}

void SpriteBatch::begin()
{
}

void SpriteBatch::submitSprite(const Sprite& sprite)
{
	sprites.push_back(sprite);
}

void SpriteBatch::end()
{
	flush();
	sprites.clear();
}

std::vector<DrawCommand> SpriteBatch::buildDrawCommands()
{
	std::vector<DrawCommand> commands;
	commands.reserve(sprites.size());

	for (const auto& sprite : sprites)
	{
		const DrawCommand command = builder.build(sprite);
		commands.push_back(command);
	}
	return commands;

}

void SpriteBatch::executeDrawCommands(const std::vector<DrawCommand>& commands)
{
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
}

void SpriteBatch::flush()
{
	std::vector<DrawCommand> commands = buildDrawCommands();
	executeDrawCommands(commands);
}
