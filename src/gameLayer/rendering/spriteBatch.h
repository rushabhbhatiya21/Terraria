#pragma once
#include <vector>
#include "sprite.h"
#include "spriteGeometryBuilder.h"

struct DrawCommand;

class SpriteBatch
{
private:
	std::vector<Sprite> sprites;
	SpriteGeometryBuilder& builder;

public:
	SpriteBatch(SpriteGeometryBuilder& builder);
	void begin();
	void submitSprite(const Sprite& sprite);
	void end();

private:
	std::vector<DrawCommand> buildDrawCommands();
	void executeDrawCommands(const std::vector<DrawCommand>& commands);
	void flush();
};