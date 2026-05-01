#pragma once
#include "zombie.h"
#include "droppedItem.h"
#include <blocks.h>
#include <gameMap.h>
#include <assetManager.h>
#include <helper.h>
#include <entityHolder.h>

void Zombie::render(AssetManager& assetManager)
{
	Transform2D zombieSprite = physics.transform;
	zombieSprite.w = 1;
	zombieSprite.h = 2;

	zombieSprite.pos.y -= (zombieSprite.h - physics.transform.h) / 2;

	auto aabb = getRectangleForEntity(zombieSprite, 1, 2);
	Color color = WHITE;

	if (isRedTimer > 0) { color = { 255,0,0,255 }; }

	DrawTexturePro(
		assetManager.zombie,
		getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft),
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
	changeDirTimer -= deltaTime;

	if (changeDirTimer <= 0)
	{
		currentState = STATE_WONDERING;
		changeDirTimer = 3.f;
	}
	else
	{
		currentState = STATE_IDLE;
	}

	if (life <= 0)
	{
		currentState = STATE_DEAD;
	}

	if (isRedTimer > 0)
	{
		isRedTimer -= deltaTime;
	}

	//if (abs(entityUpdateData.playerPosition.x - getPosition().x) >= 1.f)
	//{
	//	moveSpeed = 1.f;
	//	animation.setAnimation(1);
	//}
	//else
	//{
	//	moveSpeed = 0;
	//	animation.setAnimation(0);
	//}

	//Vector2 dir = entityUpdateData.playerPosition - getPosition();
	//dir = Vector2Normalize(dir);

	////printf("dir.x: %f\n", dir.x);
	//if (dir.x < 0)
	//{
	//	isFacingRight = false;
	//}
	//else
	//{
	//	isFacingRight = true;
	//}

	//getPosition().x += dir.x * moveSpeed * deltaTime;

	//if (moveSpeed && shouldStepUp(entityUpdateData.playerPosition, entityUpdateData.gameMap))
	//{
	//	physics.jump(10);
	//}

	// set animation
	if (!physics.downTouch)
	{
		animations.setAnimation(0);
	}
	else
	{
		animations.setAnimation(1);
	}

	// set variable for flipping sprite
	if (moveSpeed >= 0)
	{
		animations.movingLeft = false;
	}
	else
	{
		animations.movingLeft = true;
	}

	switch (currentState)
	{
	case STATE_IDLE:
	{
		printf("state changed to idle\n");
		//if (getRandomChance(entityUpdateData.rng, 0.3))
		//{
		//	changeDirTimer = 1.f;
		//	moveSpeed = 0.f;
		//}
		//else
		//{
		//	moveSpeed = defaultSpeed;
		//}
		break;
	}


	case STATE_WONDERING:
	{
		printf("state changed to wondering\n");
		moveSpeed *= -1;
		printf("movesped: %f\n", moveSpeed);
		break;
	}

	case STATE_CHASING:
	case STATE_ATTACK:
	case STATE_HURT:
	case STATE_DEAD:
		dropLoot(entityUpdateData.entityHolder, Block::boneChest);
		break;
	default:
		break;
	}

	if (moveSpeed)
	{
		getPosition().x += deltaTime * moveSpeed;
	}

	animations.update(deltaTime, 0.08f, 7);

	return true;
}

void Zombie::dropLoot(EntityHolder& entityHolder, int type)
{
	DroppedItem droppedItem;
	droppedItem.teleport(getPosition());

	// make it drop rarer chests with low chance
	droppedItem.itemType = type;
	droppedItem.physics.velocity.y = -3.f;

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.entities[id] = std::make_unique<DroppedItem>(droppedItem);
}


bool Zombie::shouldStepUp(Vector2 playerPos, GameMap& gameMap)
{
	Vector2 zTotPlayerDirection = playerPos - getPosition();

	int nextX = int(getPosition().x) + 1;
	int prevX = int(getPosition().x) - 1;

	auto bNext = gameMap.getBlockSafe(nextX, getPosition().y);
	auto bPrev = gameMap.getBlockSafe(prevX, getPosition().y);

	if (bNext && zTotPlayerDirection.x >= 0 && isCollidable(bNext->type))
	{
		return true;
	}

	if (bPrev && zTotPlayerDirection.x < 0 && isCollidable(bPrev->type))
	{
		return true;
	}
	return false;
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
