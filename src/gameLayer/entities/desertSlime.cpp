#pragma once
#include "desetSlime.h"
#include <assetManager.h>
#include <helper.h>
#include <randomStuff.h>

void DesertSlime::render(AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	float facingValue = isFacingRight ? 32 : -32;

	DrawTexturePro(
		assetManager.desertSlime,
		getTextureAtlas(animation.positionX, animation.positionY, facingValue, 32),
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);
}

bool DesertSlime::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	changeStateTimer -= deltaTime;

	if (changeStateTimer < 0)
	{
		changeStateTimer = getRandomFloat(entityUpdateData.rng, 1, 7);

		float distance = Vector2Distance(getPosition(), entityUpdateData.playerPosition);

		if (distance < 20.f)
		{
			if (getRandomChance(entityUpdateData.rng, 0.8f))
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
		animation.setAnimation(0); // on ground
	}
	else
	{
		animation.setAnimation(1); // in air
	}

	jumpTimer -= deltaTime;

	switch (currentState)
	{
	case STATE_WONDERING:
		if (jumpTimer < 0)
		{
			jumpTimer = getRandomFloat(entityUpdateData.rng, 3, 12);
			physics.jump(10);
			moveSpeed = getRandomFloat(entityUpdateData.rng, -7, 7);
		}
		break;

	case STATE_CHASING:
		if (jumpTimer < 0)
		{
			jumpTimer = getRandomFloat(entityUpdateData.rng, 2, 7);
			physics.jump(10);

			if (entityUpdateData.playerPosition.x > getPosition().x)
			{
				moveSpeed = getRandomFloat(entityUpdateData.rng, 1, 7);
			}
			else
			{
				moveSpeed = -getRandomFloat(entityUpdateData.rng, 1, 7);
			}
		}
		break;

	default:
		break;
	}

	if (moveSpeed)
	{
		getPosition().x += deltaTime * moveSpeed;
	}

	animation.update(deltaTime, 0.08f, 7);

	return true;
}

Json DesertSlime::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo slime state

	return j;
}

bool DesertSlime::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}