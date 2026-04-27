#pragma once
#include "evilEye.h"
#include <assetManager.h>
#include <helper.h>
#include <randomStuff.h>
#include <raylib.h>

void EvilEye::render(AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	float facingValue = isFacingRight ? 16 : -16;

	DrawTexturePro(
		assetManager.evilEye,
		getTextureAtlas(0, 0, facingValue, 16),
		aabb, // dest
		{ 0, 0 }, // origin top-left corner
		0.f, // rotation
		WHITE // tint
	);
}

bool EvilEye::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	// animation logic
	baseY = Lerp(baseY, entityUpdateData.groundDistance - 2.f, deltaTime * 1.f); // 5.f controls how fast it adjusts
	getPosition().y = baseY + sin(time * animationSpeed) * 1.f;
	physics.velocity.y = 0;

	if (getPosition().x > 10)
		getPosition().x += deltaTime * moveSpeed;

	time += deltaTime;
	return true;
}


//bool EvilEye::update(float deltaTime, EntityUpdateData entityUpdateData)
//{
//	changeStateTimer -= deltaTime;
//	changeStateTimer = 0;
//
//	if (changeStateTimer <= 0)
//	{
//		//changeStateTimer = getRandomFloat(entityUpdateData.rng, 1, 7);
//		changeStateTimer = 0;
//
//		float distance = Vector2Distance(getPosition(), entityUpdateData.playerPosition);
//
//		if (distance < 10.f)
//		{
//			if (getRandomChance(entityUpdateData.rng, 0.8f))
//			{
//				currentState = STATE_CHASING;
//			}
//			else
//			{
//				currentState = STATE_WONDERING;
//			}
//		}
//		else
//		{
//			currentState = STATE_WONDERING;
//		}
//	}
//
//	float tempY = 0;
//
//	switch (currentState)
//	{
//		case STATE_WONDERING:
//			// animation logic
//			baseY = Lerp(baseY, entityUpdateData.groundDistance - 2.f, deltaTime * 5.f); // 5.f controls how fast it adjusts
//			getPosition().y = baseY + sin(time * animationSpeed) * 1.f;
//			physics.velocity.y = 0;
//			printf("%f %f\n", baseY, entityUpdateData.groundDistance);
//
//			//if (entityUpdateData.shouldStepUp)
//			//{
//			//	printf("stepping up...\n");
//			//	baseY -= 1.f;
//			//}
//			time += deltaTime;
//
//			if (changeStateTimer <= 0)
//				moveSpeed = getRandomFloat(entityUpdateData.rng, -7, 7);
//			break;
//
//		case STATE_CHASING:
//			moveSpeed = entityUpdateData.playerPosition.x > getPosition().x ? 7.f : -7.f;
//			break;
//
//		default:
//			break;
//	}
//
//	if (moveSpeed)
//	{
//		getPosition().x += deltaTime * moveSpeed;
//	}
//
//	return true;
//}