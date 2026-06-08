#include "evilEye.h"
#include "evilEye.h"
#include <helper.h>
#include <assetManager.h>
#include <player.h>
#include "../../combat/combatSystem.h"

void EvilEye::drawSprite(AssetManager& assetManager)
{
	Transform2D evilEyeSprite = physics.transform;

	//static constexpr float phaseOneOffsetX = 1.25f;
	//static constexpr float phaseOneOffsetY = 2.75f;

	//static constexpr float phaseOneTextureW = 2.72f;
	//static constexpr float phaseOneTextureH = 4.f;

	//static constexpr float phaseOneOriginX = 1.2f;
	//static constexpr float phaseOneOriginY = 2.5f;

	// position below center 
	evilEyeSprite.pos.x += 1.25f;
	evilEyeSprite.pos.y += 2.7f;

	float cellSizeX = 0;
	float cellSizeY = 0;
	Texture2D tex = {};

	if (currentPhase == EvilEyePhase::ONE)
	{
		cellSizeX = PHASE_ONE_CELL_SIZE_X;
		cellSizeY = PHASE_ONE_CELL_SIZE_Y;
		tex = assetManager.evilEyePhase1;
	}
	else
	{
		cellSizeX = PHASE_TWO_CELL_SIZE_X;
		cellSizeY = PHASE_TWO_CELL_SIZE_Y;
		tex = assetManager.evilEyePhase2;
	}

	// draw size in scene
	auto aabb = getRectangleForEntity(evilEyeSprite, 2.72f, 4.f);

	DrawTexturePro(
		tex,
		getTextureAtlas(animations.positionX, animations.positionY, cellSizeX, cellSizeY),
		aabb, // dest
		{ 1.2f, 2.5f }, // origin
		rotation, // rotation
		WHITE // tint
	);

	if (flashTimer > 0)
	{
		float flash = (flashTimer > 0) ? 1.0f : 0.0f;

		BeginShaderMode(assetManager.flashShader);

		SetShaderValue(
			assetManager.flashShader,
			GetShaderLocation(assetManager.flashShader, "flash"),
			&flash,
			SHADER_UNIFORM_FLOAT
		);

		DrawTexturePro(
			tex,
			getTextureAtlas(animations.positionX, animations.positionY, cellSizeX, cellSizeY),
			aabb,
			{ 1.2f, 2.5f }, // origin
			rotation,
			Color{ 255,255,255,255 }
		);
		EndShaderMode();
	}
}

// done - keeps on dashing if in range, have 3 dashes and hover for some time
bool EvilEye::update(float deltaTime, EntityUpdateData& data)
{
	if (stateChangeTimer > 0) stateChangeTimer -= deltaTime;

	Vector2 facingDirection = data.player.getPosition() - getPosition();

	isColliding = physics.transform.intersectTransform(data.player.physics.transform);

	// todo: have player flash on hit
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
		if (dashCounter >= MAX_DASH_COUNT)
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
		if (Vector2Distance(getPosition(), dashPosition) < DASH_POSITION_EPSILON)
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
		if (dashCounter < MAX_DASH_COUNT)
		{
			enterState(EvilEyeState::POSITION_FOR_DASH, data);
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
			data.player.getPosition().y - HOVER_RANGE
		};
		moveDirection = hoverTarget - getPosition();
		moveSpeed = HOVER_SPEED;
		break;
	}
	case EvilEyeState::POSITION_FOR_DASH:
	{
		moveDirection = dashPosition - getPosition();
		moveSpeed = POSITION_SPEED;
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
		moveSpeed = DASH_SPEED;
		break;
	}
	case EvilEyeState::DASH_RECOVER:
	{
		moveDirection = dashDirection;
		moveSpeed = RECOVER_SPEED;
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
		physics.velocity += steering * MOVE_ACCELERATION * deltaTime;
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
			data.player.getPosition().x + side.x * DASH_SIDE_OFFSET,
			data.player.getPosition().y + side.y * DASH_SIDE_OFFSET
		};

		break;
	}
	case EvilEyeState::DASH_WINDUP:
	{
		dashDirection = Vector2Normalize(data.player.getPosition() - getPosition());
		stateChangeTimer = DASH_WINDUP_TIME;
		break;
	}
	case EvilEyeState::DASH:
	{
		dashCounter++;
		moveDirection = dashDirection;
		stateChangeTimer = DASH_TIME;
		break;
	}
	case EvilEye::EvilEyeState::DASH_RECOVER:
	{
		stateChangeTimer = DASH_RECOVER_TIME;
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
