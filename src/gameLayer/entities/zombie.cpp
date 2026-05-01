#pragma once
#include "zombie.h"
#include <assetManager.h>
#include <helper.h>

void Zombie::render(AssetManager& assetManager)
{
	Transform2D zombieSprite = physics.transform;
	zombieSprite.w = 1;
	zombieSprite.h = 2;

	zombieSprite.pos.y -= (zombieSprite.h - physics.transform.h) / 2;

	auto aabb = getRectangleForEntity(zombieSprite, 1, 2);
	Color color = WHITE;

	if (isRedTimer > 0) { color = { 255,0,0,255 }; }

	float facingValue = isFacingRight ? 32 : -32;

	DrawTexturePro(
		assetManager.zombie,
		getTextureAtlas(animation.positionX, animation.positionY, facingValue, 64),
		aabb, // dest
		{ 0, 0 }, // origin top-left corner
		0.f, // rotation
		color // tint
	);

	//DrawTextureRec(
	//	assetManager.zombie, 
	//	(Rectangle) { 0, 0, facing, frameHeight }, 
	//	getPosition(), 
	//	WHITE
	//);

}

bool Zombie::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	if (isRedTimer > 0)
	{
		isRedTimer -= deltaTime;
	}

	if (abs(entityUpdateData.playerPosition.x - getPosition().x) >= 1.f)
	{
		moveSpeed = 1.f;
		animation.setAnimation(1);
	}
	else
	{
		moveSpeed = 0;
		animation.setAnimation(0);
	}

	Vector2 dir = entityUpdateData.playerPosition - getPosition();
	dir = Vector2Normalize(dir);

	//printf("dir.x: %f\n", dir.x);
	if (dir.x < 0)
	{
		isFacingRight = false;
	}
	else
	{
		isFacingRight = true;
	}

	getPosition().x += dir.x * moveSpeed * deltaTime;

	if (moveSpeed && entityUpdateData.shouldStepUp)
	{
		physics.jump(10);
	}

	if (!physics.downTouch)
	{
		animation.setAnimation(0);
	}

	animation.update(deltaTime, 0.08f, 7);

	return true;
}

Json Zombie::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo zombie state

	return j;
}

bool Zombie::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
