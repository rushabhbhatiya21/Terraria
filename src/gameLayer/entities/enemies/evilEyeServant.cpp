#include "evilEyeServant.h"
#include <assets/assetManager.h>
#include <helper.h>
#include <player.h>
#include <combat/combatSystem.h>

//using Engine::AssetManager;

void EvilEyeServant::drawSprite(Engine::AssetManager& assetManager)
{
	auto& tex = assetManager.evilEyeServant;
	Rectangle aabb = getRectangleForEntity(physics.transform, spriteScale, spriteScale); // bottom-mid

	//DrawTexturePro(
	//	tex,
	//	getTextureAtlas(animations.positionX, animations.positionY, 16, 16, animations.movingLeft),
	//	aabb, // dest
	//	{ 0.f, spriteScale / 2 }, // origin (aabb - already mid so no need to touch x)
	//	rotation, // rotation
	//	WHITE // tint
	//);

	if (currentState == ServantState::DEAD_ANIM)
	{
		if (delayTimer <= 0 && !isFlashTimerSet)
		{
			blinkTimer = DEAD_TIMER / 3.f - .2f; // = .3f
			isFlashTimerSet = true;
		}

		if (blinkTimer <= 0 && isFlashTimerSet)
		{
			delayTimer = .2f;
			isFlashTimerSet = false;
		}
	}
}

bool EvilEyeServant::update(float deltaTime, EntityUpdateData& data)
{
	if (delayTimer > 0) delayTimer -= deltaTime;
	if (stateChangeTimer > 0) stateChangeTimer -= deltaTime;

	// state transition
	if (!wasColliding && 
		physics.transform.intersectTransform(data.player.physics.transform) && 
		currentState != ServantState::DEAD_ANIM &&
		currentState != ServantState::DEAD
	)
	{
		life = 0;
		isColliding = true;
		DamageInfo info;
		info.attacker = this;
		info.hitDirection = Vector2Normalize(physics.velocity);
		CombatSystem::applyDamage(&data.player, info);
	}

	if (life <= 0)
	{
		enterState(ServantState::DEAD_ANIM);
	}

	wasColliding = isColliding;

	if (currentState == ServantState::DEAD_ANIM && stateChangeTimer <= 0)
	{
		enterState(ServantState::DEAD);
	}

	// movement
	switch (currentState)
	{
	case EvilEyeServant::ServantState::CHASING:
	{
		moveDirection = data.player.getPosition() - getPosition();
		rotation = atan2f(moveDirection.y, moveDirection.x) * RAD2DEG;
		break;
	}
	case EvilEyeServant::ServantState::DEAD_ANIM:
	{
		moveSpeed = 0;
		break;
	}
	case EvilEyeServant::ServantState::DEAD:
	{
		// kill entity
		return false;
	}
	default:
		break;
	}

	if (Vector2LengthSqr(moveDirection) > 0.0001f)
	{
		moveDirection = Vector2Normalize(moveDirection);
	}

	if (moveSpeed != 0.f && moveDirection != Vector2{ 0,0 })
	{
		Vector2 desiredVelocity = moveDirection * moveSpeed;
		Vector2 steering = desiredVelocity - physics.velocity;
		physics.velocity += steering * 3.f * deltaTime;
		physics.transform.pos += physics.velocity * deltaTime;
	}

	return true;
}

void EvilEyeServant::enterState(ServantState newState)
{
	if (currentState == newState)
		return;

	previousState = currentState;
	currentState = newState;

	switch (currentState)
	{
	case EvilEyeServant::ServantState::CHASING:
		break;
	case EvilEyeServant::ServantState::DEAD_ANIM:
	{
		physics.velocity = Vector2Zero();
		stateChangeTimer = DEAD_TIMER;
		break;
	}
	case EvilEyeServant::ServantState::DEAD:
		break;
	default:
		break;
	}
}

void EvilEyeServant::dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder)
{
}

Json EvilEyeServant::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);
	return j;
}

bool EvilEyeServant::loadFromJson(Json& j)
{
	*this = {};
	bool rez = loadCommonEntityStuffFromJson(j);
	setColliderSize();
	return rez;
}
