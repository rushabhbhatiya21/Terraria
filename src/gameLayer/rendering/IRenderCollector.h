#pragma once

namespace Engine
{
	struct Sprite;

	class IRenderCollector
	{
	public:
		virtual ~IRenderCollector() = default;

		virtual void submitSprite(const Sprite& sprite) = 0;
	};
}