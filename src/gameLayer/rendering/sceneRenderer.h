#pragma once
#include "IRenderCollector.h"
#include <math/cam.h>


namespace Engine
{
	enum class RenderPass
	{
		Background,
		World,
		UI
	};

	struct Sprite;
	struct ColoredRect;
	struct Line;
	struct OutlinedRect;
	struct Circle;
	struct OutlinedCircle;

	class SpriteBatch;
	class IRenderBackend;
}

class SceneRenderer : public Engine::IRenderCollector
{
public:
	SceneRenderer(Engine::SpriteBatch& spriteBatch, Engine::IRenderBackend& backend);
	void beginFrame();
	void beginPass(Engine::RenderPass pass, Engine::Cam& camera);
	void endPass();
	void endFrame();

	void submitSprite(const Engine::Sprite& sprite) override;
	void submitRect(const Engine::ColoredRect& rect) override;
	void submitLine(const Engine::Line& line) override;
	void submitOutlinedRect(const Engine::OutlinedRect& rect) override;
	void submitCircle(const Engine::Circle& circle) override;
	void submitOutlinedCircle(const Engine::OutlinedCircle& circle) override;

private:
	Engine::SpriteBatch& spriteBatch;
	Engine::IRenderBackend& backend;
};