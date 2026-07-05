#pragma once
#include <vector>
#include "sprite.h"

class SpriteBatch
{
private:
	std::vector<Sprite> sprites;

public:
	void begin();
	void submit(const Sprite& sprite);
	void end();

private:
	void flush();
};