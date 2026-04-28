#include "player.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>
#include <inventory.h>

void Player::render(AssetManager& assetManager)
{
	Transform2D playerSprite = physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;

	playerSprite.pos.y -= (playerSprite.h - physics.transform.h) / 2;

	auto aabb = playerSprite.getAABB();

	float facingDirValue = (float)assetManager.player.width * (isFacingRight ? 1 : -1);

	DrawTexturePro(
		assetManager.player,
		{ 0,0,facingDirValue,(float)assetManager.player.height },
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);

	DrawRectangleLinesEx(
		aabb,
		0.1f,
		{ 20,101,250,120 }
	);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	return true;
}