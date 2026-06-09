#include "evilEye.h"
#include "evilEye.h"
#include <helper.h>
#include <assetManager.h>
#include <player.h>
#include "../../combat/combatSystem.h"

void EvilEye::drawSprite(AssetManager& assetManager)
{
	Transform2D evilEyeSprite = physics.transform;

	// position below center 
	evilEyeSprite.pos.x += 1.25f;
	evilEyeSprite.pos.y += 2.7f;

	Texture2D tex = {};

	switch (currentPhase)
	{
	case EvilEye::EvilEyePhase::ONE:
		tex = assetManager.evilEyePhase1;
		break;
	case EvilEye::EvilEyePhase::TWO:
		tex = assetManager.evilEyePhase2;
		break;
	case EvilEyePhase::DEAD:
		tex = assetManager.evilEyePieces;
		break;
	default:
		break;
	}

	// draw size in scene
	auto aabb = getRectangleForEntity(evilEyeSprite, 2.72f, 4.f);

	DrawTexturePro(
		tex,
		getTextureAtlas(animations.positionX, animations.positionY, currentPhaseData->cellSizeX, currentPhaseData->cellSizeY),
		aabb, // dest
		{ 1.2f, 2.5f }, // origin
		rotation, // rotation
		WHITE // tint
	);
}

// done - keeps on dashing if in range, have 3 dashes and hover for some time
// todo: implement new transition state, from phase 1 to 2
bool EvilEye::update(float deltaTime, EntityUpdateData& data)
{
	if (life <= (float)stats.defensive.maxHealth / 2)
	{
		currentPhase = EvilEyePhase::TWO;
		currentPhaseData = &PHASE_2;
	}

	if (stateChangeTimer > 0) stateChangeTimer -= deltaTime;

	Vector2 facingDirection = data.player.getPosition() - getPosition();

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
		rotation = atan2f(dashDirection.y, dashDirection.x) * RAD2DEG - 90.f;
	}
	else
	{
		rotation = atan2f(facingDirection.y, facingDirection.x) * RAD2DEG - 90.f;
	}

	// state transitions
	if (currentState == EvilEyeState::HOVERING && stateChangeTimer <= 0)
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
		if (Vector2Distance(getPosition(), dashPosition) < currentPhaseData->dashPositionEpsilon)
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
				enterState(EvilEyeState::DASH, data);
			}
		}

		enterState(EvilEyeState::HOVERING, data);
	}

	// movement
	switch (currentState)
	{
	case EvilEyeState::HOVERING:
	{
		hoverTarget =
		{
			data.player.getPosition().x + getRandomFloat(data.rng, -1.f, 1.f),
			data.player.getPosition().y - currentPhaseData->hoverRange
		};
		moveDirection = hoverTarget - getPosition();
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

	if (Vector2LengthSqr(moveDirection) > 0.0001f)
	{
		moveDirection = Vector2Normalize(moveDirection);
	}

	if (moveSpeed == 0.f)
	{
		physics.velocity = Vector2Lerp(physics.velocity, Vector2Zero(), 10.f * deltaTime);
	}

	if (moveSpeed != 0.f && moveDirection != Vector2{ 0,0 })
	{
		Vector2 desiredVelocity = moveDirection * moveSpeed;
		Vector2 steering = desiredVelocity - physics.velocity;
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
		Vector2 toPlayer = Vector2Normalize(data.player.getPosition() - getPosition());
		Vector2 side = { -toPlayer.y,  toPlayer.x };
		dashPosition =
		{
			data.player.getPosition().x + side.x * currentPhaseData->dashSideOffset,
			data.player.getPosition().y + side.y * currentPhaseData->dashSideOffset
		};

		break;
	}
	case EvilEyeState::DASH_WINDUP:
	{
		dashDirection = Vector2Normalize(data.player.getPosition() - getPosition());
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
	case EvilEye::EvilEyeState::DASH_RECOVER:
	{
		stateChangeTimer = currentPhaseData->dashRecoverTime;
		break;
	}
	case EvilEye::EvilEyeState::DEAD:
		break;
	default:
		break;
	}

}

void EvilEye::dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder)
{
}

Json EvilEye::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo zombie state

	return j;
}

bool EvilEye::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
