#pragma once
#include "IRenderCollector.h"
#include <raylib.h>

namespace Engine
{
	enum class RenderPass
	{
		Background,
		World,
		UI
	};

	struct Sprite;
	class SpriteBatch;
	class IRenderBackend;
}

class SceneRenderer : public Engine::IRenderCollector
{
public:
	SceneRenderer(Engine::SpriteBatch& spriteBatch, Engine::IRenderBackend& backend);
	void beginFrame();
	void beginPass(Engine::RenderPass pass, Camera2D& camera);
	void endPass();
	void endFrame();
	void submitSprite(const Engine::Sprite& sprite) override;

private:
	Engine::SpriteBatch& spriteBatch;
	Engine::IRenderBackend& backend;
};