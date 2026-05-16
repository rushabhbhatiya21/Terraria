#include "enemy.h"
#include <assetManager.h>

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

	if (hurtTimer >= 0)
	{
		float progress = 1.f - (hurtTimer / 2);
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

bool Enemy::updateHealthBar(float deltatime)
{
	if (hurtTimer >= 0)
	{
		hurtTimer -= deltatime;
	}

	if (hurtTimer < 0)
	{
		damageTaken = 0;
	}

	return false;
}