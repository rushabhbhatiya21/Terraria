#include "enemy.h"
#include <assetManager.h>

void Enemy::render(AssetManager& assetManager)
{
	bool flashing = flashTimer > 0;

	if (flashing)
	{
		float flash = 1.f;

		BeginShaderMode(assetManager.flashShader);

		SetShaderValue(
			assetManager.flashShader,
			assetManager.flashShaderLocation,
			&flash,
			SHADER_UNIFORM_FLOAT
		);
	}

	drawSprite(assetManager);

	if (flashing)
	{
		EndShaderMode();
	}

}

void Enemy::renderHealthBar(AssetManager& assetManager)
{
	float healthWidth = 1.6f;
	float healthHeight = .4f;
	baseX = physics.transform.getTop().x - (healthWidth / 2);

	// enemy middle = healthBar middle
	Rectangle healthBarPos = Rectangle
	{
		baseX,
		physics.transform.getTop().y - .6f,
		healthWidth,
		healthHeight
	};

	DrawTexturePro(
		assetManager.healthBar,
		{ 0,0,(float)assetManager.healthBar.width, (float)assetManager.healthBar.height },
		healthBarPos,
		{ 0,0 },
		0,
		WHITE
	);

	healthBarPos.width = life * healthWidth / getMaxLife();

	DrawTexturePro(
		assetManager.health,
		{ 0,0,(float)assetManager.health.width, (float)assetManager.health.height },
		healthBarPos,
		{ 0,0 },
		0,
		WHITE
	);

	if (damageTakenHealthBarTimer >= 0)
	{
		float progress = 1.f - (damageTakenHealthBarTimer / 2);
		float fade = 1.f - pow(progress, 3.f);

		Rectangle r{
			baseX + healthBarPos.width,
			healthBarPos.y,
			damageTaken * fade * healthWidth / getMaxLife(),
			healthHeight
		};

		DrawTexturePro(
			assetManager.health,
			{ 0,0,(float)assetManager.health.width, (float)assetManager.health.height },
			r,
			{ 0,0 },
			0,
			Color{ 255,255,255,80 }
		);
	}
}

bool Enemy::updateHealthBar(float deltaTime)
{
	if (flashTimer > 0) flashTimer -= deltaTime;

	if (life <= 0)
	{
		isAlive = false;
	}

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