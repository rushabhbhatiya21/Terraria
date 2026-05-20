#pragma once
#include "slime.h"
#include "../droppedItem.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>
#include <items/blocks.h>
#include <player.h>

void Slime::render(AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);
	Color color = WHITE;

	if (isRedTimer > 0) { color = { 255,0,0,255 }; }

	float facingValue = isFacingRight ? 32 : -32;

	DrawTexturePro(
		assetManager.slime,
		getTextureAtlas(animations.positionX, animations.positionY, facingValue, 32),
		aabb, // dest
		{ 0, 0 }, // origin top-left corner
		0.f, // rotation
		color // tint
	);
}

bool Slime::update(float deltaTime, EntityUpdateData& data)
{
	if (isRedTimer > 0)
	{
		isRedTimer -= deltaTime;
	}

	changeStateTimer -= deltaTime;

	if (life <= 0)
	{
		currentState = STATE_DEAD;
	}

	if (changeStateTimer < 0)
	{
		changeStateTimer = getRandomFloat(data.rng, 1, 7);

		float distance = Vector2Distance(getPosition(), data.player.getPosition());

		if (distance < 20.f)
		{
			if (getRandomChance(data.rng, 0.8f))
			{
				currentState = STATE_CHASING;
			}
			else
			{
				currentState = STATE_WONDERING;
			}
		}
		else
		{
			currentState = STATE_WONDERING;
		}
	}

	if (physics.downTouch)
	{
		moveSpeed = 0.f;
		animations.setAnimation(0); // on ground
	}
	else
	{
		animations.setAnimation(1); // in air
	}

	jumpTimer -= deltaTime;

	switch (currentState)
	{
		case STATE_WONDERING:
			if (jumpTimer < 0)
			{
				jumpTimer = getRandomFloat(data.rng, 3, 12);
				physics.jump(10);
				moveSpeed = getRandomFloat(data.rng, -7, 7);
			}
			break;

		case STATE_CHASING:
			if (jumpTimer < 0)
			{
				jumpTimer = getRandomFloat(data.rng, 2, 7);
				physics.jump(7);

				if (data.player.getPosition().x > getPosition().x)
				{
					moveSpeed = getRandomFloat(data.rng, 1, 7);
				}
				else
				{
					moveSpeed = -getRandomFloat(data.rng, 1, 7);
				}
			}
			break;

		case STATE_DEAD:
		{
			// drop loop
			dropLoot(data.entityHolder, Block::woodenChest);
			return false;
		}

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

void Slime::dropLoot(EntityHolder& entityHolder, int type)
{
	DroppedItem droppedItem;
	droppedItem.teleport(getPosition());

	// make it drop rarer chests with low chance
	droppedItem.itemType = type;
	droppedItem.physics.velocity.y = -3.f;

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.droppedItems.push_back(&droppedItem);
	entityHolder.entities[id] = std::make_unique<DroppedItem>(droppedItem);
}

Json Slime::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo slime state

	return j;
}

bool Slime::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
