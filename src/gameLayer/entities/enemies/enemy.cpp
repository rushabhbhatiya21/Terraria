#include "enemy.h"
#include <assets/assetManager.h>
#include <rendering/sprite.h>
#include <rendering/IRenderCollector.h>

void Enemy::render(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	//bool flashing = flashTimer > 0;

	//if (flashing)
	//{
	//	float flash = 1.f;

	//	BeginShaderMode(assetManager.flashShader);

	//	SetShaderValue(
	//		assetManager.flashShader,
	//		assetManager.flashShaderLocation,
	//		&flash,
	//		SHADER_UNIFORM_FLOAT
	//	);
	//}

	//bool blinking = blinkTimer > 0;

	//// flashing takes priority
	//if (!flashing && blinking)
	//{
	//	return; // do not draw sprite
	//}

	drawSprite(assetManager, collector);

	//if (flashing)
	//{
	//	EndShaderMode();
	//}
}

void Enemy::renderHealthBar(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	float healthWidth = 1.6f;
	float healthHeight = .4f;
	baseX = physics.transform.getTop().x - (healthWidth / 2);

	// enemy middle = healthBar middle
	Engine::Rect healthBarPos = Engine::Rect
	{
		baseX,
		physics.transform.getTop().y - .6f,
		healthWidth,
		healthHeight
	};

	//DrawTexturePro(
	//	assetManager.healthBar,
	//	{ 0,0,(float)assetManager.healthBar.getWidth(), (float)assetManager.healthBar.getHeight()},
	//	healthBarPos,
	//	{ 0,0 },
	//	0,
	//	WHITE
	//);

	Engine::Sprite healthBarSprite
	{
		&assetManager.healthBar,
		nullptr,
		{ 0,0,(float)assetManager.healthBar.getWidth(), (float)assetManager.healthBar.getHeight()},
		healthBarPos,
		{ 0,0 },
		0.f,
		Engine::White
	};
	collector.submitSprite(healthBarSprite);

	healthBarPos.width = life * healthWidth / stats.defensive.maxHealth;

	Engine::Color4f color = Engine::White;

	if (life > stats.defensive.maxHealth * .7f)
	{
		color = Engine::Lime;
	}
	else if (life > stats.defensive.maxHealth * .3f && life <= stats.defensive.maxHealth * .7f)
	{
		color = { 255, 200, 120, 255 };;
	}
	else
	{
		color = Engine::Red;
	}

	//DrawTexturePro(
	//	assetManager.health,
	//	{ 0,0,(float)assetManager.health.getWidth(), (float)assetManager.health.getHeight()},
	//	healthBarPos,
	//	{ 0,0 },
	//	0,
	//	color
	//);

	Engine::Sprite healthSprite
	{
		&assetManager.health,
		nullptr,
		{ 0,0,(float)assetManager.health.getWidth(), (float)assetManager.health.getHeight()},
		healthBarPos,
		{ 0,0 },
		0.f,
		Engine::White
	};
	collector.submitSprite(healthSprite);

	if (damageTakenHealthBarTimer >= 0)
	{
		float progress = 1.f - (damageTakenHealthBarTimer / 2);
		float fade = 1.f - pow(progress, 3.f);
	
		Engine::Rect r{
			baseX + healthBarPos.width,
			healthBarPos.y,
			damageTaken * fade * healthWidth / stats.defensive.maxHealth,
			healthHeight
		};

	//	DrawTexturePro(
	//		assetManager.health,
	//		{ 0,0,(float)assetManager.health.width, (float)assetManager.health.height },
	//		r,
	//		{ 0,0 },
	//		0,
	//		Color{ 255,255,255,80 }
	//	);

		Engine::Sprite healthSpriteTemp
		{
			&assetManager.health,
			nullptr,
			{ 0,0,(float)assetManager.health.getWidth(), (float)assetManager.health.getHeight()},
			r,
			{ 0,0 },
			0.f,
			Engine::Color4f{ 255,255,255,80 }
		};
		collector.submitSprite(healthSpriteTemp);
	}
}

bool Enemy::updateHealthBar(float deltaTime)
{
	if (flashTimer > 0) flashTimer -= deltaTime;

	if (blinkTimer > 0) blinkTimer -= deltaTime;

	if (damageTakenHealthBarTimer >= 0)
	{
		damageTakenHealthBarTimer -= deltaTime;
	}

	if (damageTakenHealthBarTimer < 0)
	{
		damageTaken = 0;
	}

	return false;
}

void Enemy::onHit()
{
	flashTimer = .15f;
	damageTakenHealthBarTimer = 1;
}