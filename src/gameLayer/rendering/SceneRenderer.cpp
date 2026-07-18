#include "sceneRenderer.h"
#include <window/window.h>
#include <array>
#include <rendering/spriteBatch.h>
#include <rendering/IRenderBackend.h>

SceneRenderer::SceneRenderer(Engine::SpriteBatch& spriteBatch, Engine::IRenderBackend& backend)
	: spriteBatch(spriteBatch), backend(backend)
{
}

void SceneRenderer::beginFrame()
{
	backend.beginFrame();
}

void SceneRenderer::beginPass(Engine::RenderPass pass, Engine::Cam& camera)
{
	float screenWidth = (float)Engine::getScreenWidth();
	float screenHeight = (float)Engine::getScreenHeight();

	std::array<float, 16> projection {};
	switch (pass)
	{
	case Engine::RenderPass::World:
	{
		// Raylib Camera2D: screenPos = (worldPos - target) * zoom + offset
		// Then map screen to NDC: NDC = screen * (2/size) - 1  (with Y flipped)

		float sx = 2.0f * camera.zoom / screenWidth;
		float sy = -2.0f * camera.zoom / screenHeight;
		float tx = -camera.target.x * sx + (2.0f * camera.offset.x / screenWidth - 1.0f);
		float ty = -camera.target.y * sy + (-2.0f * camera.offset.y / screenHeight + 1.0f);

		projection = {};
		projection[0] = sx;
		projection[5] = sy;
		projection[10] = 1.0f;
		projection[12] = tx;
		projection[13] = ty;
		projection[15] = 1.0f;

		backend.setProjection(projection);
		break;
	}
	case Engine::RenderPass::Background:
	case Engine::RenderPass::UI:
	{
		projection = {};
		projection[0] = 2.0f / screenWidth;
		projection[5] = -2.0f / screenHeight;
		projection[10] = 1.0f;
		projection[12] = -1.0f;
		projection[13] = 1.0f;
		projection[15] = 1.0f;

		backend.setProjection(projection);
		break;
	}

	default:
		break;
	}
}

void SceneRenderer::endPass()
{
	spriteBatch.flush(backend);
}

void SceneRenderer::endFrame()
{
	backend.endFrame();
}

void SceneRenderer::submitSprite(const Engine::Sprite& sprite)
{
	spriteBatch.submitSprite(sprite);
}
