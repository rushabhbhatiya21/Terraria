#include "spriteBatch.h"
#include "spriteBatch.h"
#include "spriteBatch.h"

void SpriteBatch::begin()
{
}

void SpriteBatch::submit(const Sprite& sprite)
{
	sprites.push_back(sprite);
}

void SpriteBatch::end()
{
	flush();
	sprites.clear();
}

void SpriteBatch::flush()
{
	for (const auto& sprite : sprites)
	{
		DrawTexturePro(
			sprite.texture,  // texture
			sprite.srcRect,  // source
			sprite.destRect, // dest
			sprite.origin,   // origin (top-left)
			sprite.rotation, // rotation
			sprite.tint
		);
	}
}
