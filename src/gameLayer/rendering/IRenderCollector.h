#pragma once

namespace Engine
{
	struct Sprite;
	struct ColoredRect;

	class IRenderCollector
	{
	public:
		virtual ~IRenderCollector() = default;

		virtual void submitSprite(const Sprite& sprite) = 0;
		virtual void submitRect(const ColoredRect& rect) = 0;
	};
}