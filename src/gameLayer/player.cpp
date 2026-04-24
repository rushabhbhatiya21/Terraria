#include "player.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>

void Player::render(AssetManager& assetManager)
{
	auto aabb = physics.transform.getAABB();

	DrawTexturePro(
		assetManager.player,
		{ 0,0,(float)assetManager.player.width,(float)assetManager.player.height },
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