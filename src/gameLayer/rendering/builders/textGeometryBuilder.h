#pragma once

namespace Engine
{
	struct Text;
	class IGeometrySink;
	class SpriteGeometryBuilder;

	class TextGeometryBuilder
	{
	public:
		explicit TextGeometryBuilder(SpriteGeometryBuilder& spriteBuilder);

		void build(const Text& text, IGeometrySink& sink);

	private:
		SpriteGeometryBuilder& m_spriteBuilder;
	};
}