#include "evilEye.h"
#include <helper.h>
#include <assets/assetManager.h>
#include <player.h>
#include "evilEyeServant.h"
#include <combat/combatSystem.h>
#include <rendering/sprite.h>
#include <rendering/IRenderCollector.h>

//using Engine::AssetManager;

void EvilEye::drawSprite(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	Transform2D evilEyeSprite = physics.transform;

	// position below center 
	evilEyeSprite.pos.x += 1.25f;
	evilEyeSprite.pos.y += 2.7f;

	const Engine::Texture* tex = nullptr;

	switch (currentPhase)
	{
	case EvilEyePhase::ONE:
		tex = &assetManager.evilEyePhase1;
		break;

	case EvilEyePhase::TWO:
		tex = &assetManager.evilEyePhase2;
		break;

	case EvilEyePhase::DEAD:
		tex = &assetManager.evilEyePieces;
		break;
	}

	assert(tex);

	// draw size in scene
	auto aabb = getRectangleForEntity(evilEyeSprite, 2.72f, 4.f);

	//DrawTexturePro(
	//	tex,
	//	getTextureAtlas(animations.positionX, animations.positionY, currentPhaseData->cellSizeX, currentPhaseData->cellSizeY),
	//	aabb, // dest
	//	{ 1.2f, 2.5f }, // origin
	//	rotation, // rotation
	//	WHITE // tint
	//);
	Engine::Sprite sprite
	{
		tex,
		nullptr,
		getTextureAtlas(animations.positionX, animations.positionY, currentPhaseData->cellSizeX, currentPhaseData->cellSizeY),
		aabb,
		{ 1.2f, 2.5f },
		rotation,
		Engine::White
	};
	collector.submitSprite(sprite);
}

// done - keeps on dashing if in range, have 3 dashes and hover for some time
// done - implement new transition state, from phase 1 to 2
bool EvilEye::update(float deltaTime, EntityUpdateData& data)
{
	if (stateChangeTimer > 0) stateChangeTimer -= deltaTime;

	//switch (currentState)
	//{
	//case EvilEye::EvilEyeState::HOVERING:
	//	printf("state: %s\n", "HOVERING");
	//	break;
	//case EvilEye::EvilEyeState::POSITION_FOR_DASH:
	//	printf("state: %s\n", "POSITION_FOR_DASH");
	//	break;
	//case EvilEye::EvilEyeState::DASH_WINDUP:
	//	printf("state: %s\n", "DASH_WINDUP");
	//	break;
	//case EvilEye::EvilEyeState::DASH:
	//	printf("state: %s\n", "DASH");
	//	break;
	//case EvilEye::EvilEyeState::DASH_RECOVER:
	//	printf("state: %s\n", "DASH_RECOVER");
	//	break;
	//case EvilEye::EvilEyeState::TRANSITION:
	//	printf("state: %s\n", "TRANSITION");
	//	break;
	//case EvilEye::EvilEyeState::DEAD:
	//	printf("state: %s\n", "DEAD");
	//	break;
	//default:
	//	break;
	//}

	Engine::Vec2 facingDirection = data.player.getPosition() - getPosition();

	// todo: hurt player faction
	isColliding = physics.transform.intersectTransform(data.player.physics.transform);

	if (isColliding && !wasColliding)
	{
		wasColliding = true;
		DamageInfo info;
		info.attacker = this;
		info.hitDirection = facingDirection;
		CombatSystem::applyDamage(&data.player, info);
	}
	
	wasColliding = isColliding;

	// look toward player
	if (currentState == EvilEyeState::DASH_WINDUP || currentState == EvilEyeState::DASH)
	{
		rotation = atan2f(dashDirection.y, dashDirection.x) * Engine::Rad2Deg - 90.f;
	}
	else
	{
		// rotation updated in movement for transition state
		if (currentState != EvilEyeState::TRANSITION)
			rotation = atan2f(facingDirection.y, facingDirection.x) * Engine::Rad2Deg - 90.f;
	}

	// state transitions
	if (life <= (float)stats.defensive.maxHealth / 2 && currentPhase != EvilEyePhase::TWO)
	{
		currentPhase = EvilEyePhase::TWO;
		currentPhaseData = &PHASE_2;
		enterState(EvilEyeState::TRANSITION, data);
	}

	if ((currentState == EvilEyeState::TRANSITION || currentState == EvilEyeState::HOVERING) && stateChangeTimer <= 0)
	{
		if (dashCounter >= currentPhaseData->maxDashCount)
		{
			dashCounter = 0;
			stateChangeTimer = getRandomFloat(data.rng, 1.f, 4.f);
		}
		else
		{
			enterState(EvilEyeState::POSITION_FOR_DASH, data);
		}
	}

	if (currentState == EvilEyeState::POSITION_FOR_DASH)
	{
		if (Engine::Vec2Distance(getPosition(), dashPosition) < currentPhaseData->dashPositionEpsilon)
		{
			enterState(EvilEyeState::DASH_WINDUP, data);
		}
	}

	if (currentState == EvilEyeState::DASH_WINDUP && stateChangeTimer <= 0)
	{
		enterState(EvilEyeState::DASH, data);
	}

	if (currentState == EvilEyeState::DASH && stateChangeTimer <= 0)
	{
		enterState(EvilEyeState::DASH_RECOVER, data);
	}

	if (currentState == EvilEyeState::DASH_RECOVER && stateChangeTimer <= 0)
	{
		if (dashCounter < currentPhaseData->maxDashCount)
		{
			if (currentPhase == EvilEyePhase::ONE)
			{
				enterState(EvilEyeState::POSITION_FOR_DASH, data);
			}
			else
			{
				dashDirection = Engine::Vec2Normalize(data.player.getPosition() - getPosition());
				enterState(EvilEyeState::DASH, data);
			}
		}
		else
		{
			servantsCount = getRandomInt(data.rng, 2, 5);
			enterState(EvilEyeState::SPAWN, data);
		}
	}

	if (currentState == EvilEyeState::SPAWN && stateChangeTimer <= 0)
	{
		if (spawnedServantsCount >= servantsCount)
		{
			servantsCount = 0;
			spawnedServantsCount = 0;
			enterState(EvilEyeState::HOVERING, data);
		}
		else
		{
			spawnServant(data);
			spawnedServantsCount++;
			stateChangeTimer = 0.3f;
		}
	}

	// movement
	switch (currentState)
	{
	case EvilEyeState::HOVERING:
	case EvilEyeState::SPAWN:
	{
		hoverTarget =
		{
			data.player.getPosition().x + getRandomFloat(data.rng, -1.f, 1.f),
			data.player.getPosition().y - currentPhaseData->hoverRange
		};
		moveDirection = hoverTarget - getPosition();

		// if in range of desired position, statechangetimer decrese speed double
		if (Engine::Vec2Length(moveDirection) <= 1.f && stateChangeTimer > 0)
			stateChangeTimer -= deltaTime;

		moveSpeed = currentPhaseData->hoverSpeed;
		break;
	}
	case EvilEyeState::POSITION_FOR_DASH:
	{
		moveDirection = dashPosition - getPosition();
		moveSpeed = currentPhaseData->positionSpeed;
		break;
	}
	case EvilEye::EvilEyeState::DASH_WINDUP:
	{
		moveSpeed = 0.f;
		break;
	}
	case EvilEye::EvilEyeState::DASH:
	{
		moveDirection = dashDirection;
		moveSpeed = currentPhaseData->dashSpeed;
		break;
	}
	case EvilEyeState::DASH_RECOVER:
	{
		moveDirection = dashDirection;
		moveSpeed = currentPhaseData->recoverSpeed;
		break;
	}
	case EvilEyeState::TRANSITION:
	{
		rotation += 800.f * deltaTime;
		rotation = fmod(rotation, 360.f);
		moveSpeed = 0;
		moveDirection = { 0,0 };
		break;
	}
	case EvilEye::EvilEyeState::DEAD:
	{
		moveSpeed = 0;
		dropLoot(0, data.rng, data.entityHolder);
		isAlive = false;
		break;
	}
	default:
		break;
	}

	if (Engine::Vec2LengthSqr(moveDirection) > 0.0001f)
	{
		moveDirection = Engine::Vec2Normalize(moveDirection);
	}

	if (moveSpeed == 0.f)
	{
		physics.velocity = Engine::Vec2Lerp(physics.velocity, Engine::Vec2Zero(), 10.f * deltaTime);
	}

	if (moveSpeed != 0.f && moveDirection != Engine::Vec2{ 0,0 })
	{
		Engine::Vec2 desiredVelocity = moveDirection * moveSpeed;
		Engine::Vec2 steering = desiredVelocity - physics.velocity;
		physics.velocity += steering * currentPhaseData->moveAcceleration * deltaTime;
		physics.transform.pos += physics.velocity * deltaTime;
	}

	animations.setAnimation(0);
	animations.update(deltaTime, 0.18f, 3);

	return true;
}

void EvilEye::enterState(EvilEyeState newState, EntityUpdateData& data)
{
	if (currentState == newState)
		return;

	previousState = currentState;
	currentState = newState;

	switch (currentState)
	{
	case EvilEyeState::HOVERING:
	{
		stateChangeTimer = getRandomFloat(data.rng, 1.f, 4.f);
		break;
	}
	case EvilEyeState::POSITION_FOR_DASH:
	{
		Engine::Vec2 toPlayer = Engine::Vec2Normalize(data.player.getPosition() - getPosition());
		Engine::Vec2 side = { -toPlayer.y,  toPlayer.x };
		dashPosition =
		{
			data.player.getPosition().x + side.x * currentPhaseData->dashSideOffset,
			data.player.getPosition().y + side.y * currentPhaseData->dashSideOffset
		};

		break;
	}
	case EvilEyeState::DASH_WINDUP:
	{
		dashDirection = Engine::Vec2Normalize(data.player.getPosition() - getPosition());
		stateChangeTimer = currentPhaseData->dashWindupTime;
		break;
	}
	case EvilEyeState::DASH:
	{
		dashCounter++;
		moveDirection = dashDirection;
		stateChangeTimer = currentPhaseData->dashTime;
		break;
	}
	case EvilEyeState::DASH_RECOVER:
	{
		stateChangeTimer = currentPhaseData->dashRecoverTime;
		break;
	}
	case EvilEyeState::SPAWN:
		break;
	case EvilEyeState::TRANSITION:
	{
		stateChangeTimer = 2.f;
		break;
	}
	case EvilEye::EvilEyeState::DEAD:
		break;
	default:
		break;
	}

}

void EvilEye::spawnServant(EntityUpdateData& data)
{
	float offset = getRandomFloat(data.rng, 0.f, 360.f);
	float angle = (offset + 120.f * spawnedServantsCount) * Engine::Deg2Rad;
	Engine::Vec2 spawnVel = { cosf(angle) * 5.f, sinf(angle) * 5.f };
	spawnManager.spawnEnemy<EvilEyeServant>(data.entityHolder, getPosition(), spawnVel);
}

void EvilEye::dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder)
{
}

Json EvilEye::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);
	return j;
}

bool EvilEye::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
