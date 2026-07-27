#pragma once

namespace Engine
{
	struct Sprite;
	struct ColoredRect;
	struct Line;
	struct OutlinedRect;
	struct Circle;
	struct OutlinedCircle;
	struct RoundedRect;

	class IRenderCollector
	{
	public:
		virtual ~IRenderCollector() = default;

		virtual void submitSprite(const Sprite& sprite) = 0;
		virtual void submitRect(const ColoredRect& rect) = 0;
		virtual void submitLine(const Line& line) = 0;
		virtual void submitOutlinedRect(const OutlinedRect& rect) = 0;
		virtual void submitCircle(const Circle& circle) = 0;
		virtual void submitOutlinedCircle(const OutlinedCircle& circle) = 0;
		virtual void submitRoundedRect(const RoundedRect& roundedRect) = 0;
	};
}