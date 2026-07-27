#include "sceneRenderer.h"
#include "sceneRenderer.h"
#include <array>
#include <window/window.h>
#include "IRenderBackend.h"
#include <rendering/batching/spriteBatch.h>


SceneRenderer::SceneRenderer(std::unique_ptr<Engine::IRenderBackend> backend)
	: m_backend(std::move(backend))
{
}

void SceneRenderer::initialize()
{
	if (m_backend)
		m_backend->initialize();
}

void SceneRenderer::beginFrame()
{
	m_backend->beginFrame();
}

void SceneRenderer::beginPass(Engine::RenderPass pass, const Engine::Cam& camera)
{
	float screenWidth = (float)Engine::getScreenWidth();
	float screenHeight = (float)Engine::getScreenHeight();

	std::array<float, 16> projection{};
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

		m_backend->setProjection(projection);
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

		m_backend->setProjection(projection);
		break;
	}

	default:
		break;
	}
}

void SceneRenderer::endPass()
{
	m_spriteBatch.flush(*m_backend);
}

void SceneRenderer::endFrame()
{
	m_backend->endFrame();
}

void SceneRenderer::submitSprite(const Engine::Sprite& sprite)
{
	m_spriteBatch.submitSprite(sprite);
}

void SceneRenderer::submitRect(const Engine::ColoredRect& rect)
{
	m_spriteBatch.submitRect(rect);
}

void SceneRenderer::submitLine(const Engine::Line& line)
{
	m_spriteBatch.submitLine(line);
}

void SceneRenderer::submitOutlinedRect(const Engine::OutlinedRect& rect)
{
	m_spriteBatch.submitOutlinedRect(rect);
}

void SceneRenderer::submitCircle(const Engine::Circle& circle)
{
	m_spriteBatch.submitCircle(circle);
}

void SceneRenderer::submitOutlinedCircle(const Engine::OutlinedCircle& circle)
{
	m_spriteBatch.submitOutlinedCircle(circle);
}

void SceneRenderer::submitRoundedRect(const Engine::RoundedRect& roundedRect)
{
	m_spriteBatch.submitRoundedRect(roundedRect);
}
