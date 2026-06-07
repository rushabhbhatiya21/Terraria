#include "evilEye.h"
#include "evilEye.h"
#include <helper.h>
#include <assetManager.h>
#include <player.h>

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

// todo: keeps on dashing if in range, have 3 dashes and hover for some time
bool EvilEye::update(float deltaTime, EntityUpdateData& data)
{
	if (stateChangeTimer > 0) stateChangeTimer -= deltaTime;

	Vector2 facingDirection = data.player.getPosition() - getPosition();

	// look toward player
	if (currentState == EvilEyeState::DASH_WINDUP || currentState == EvilEyeState::DASH)
	{
		rotation = atan2f(dashDirection.y, dashDirection.x) * RAD2DEG - 90.f;
	}
	else
	{
		rotation = atan2f(facingDirection.y, facingDirection.x) * RAD2DEG - 90.f;
	}

	// to player distance
	float dist = Vector2Length(facingDirection);

	// state transitions
	if (currentState == EvilEyeState::HOVERING && dist <= DASH_INIT_RANGE)
	{
		printf("entering state hovering.\n");
		enterState(EvilEyeState::POSITION_FOR_DASH, data);
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
		printf("entering state dash.\n");
		enterState(EvilEyeState::DASH, data);
	}

	if (currentState == EvilEyeState::DASH && stateChangeTimer <= 0)
	{
		printf("entering state dash recover.\n");
		enterState(EvilEyeState::DASH_RECOVER, data);
	}

	if (currentState == EvilEyeState::DASH_RECOVER && stateChangeTimer <= 0)
	{
		printf("entering state hovering.\n");
		enterState(EvilEyeState::HOVERING, data);
	}

	// movement
	switch (currentState)
	{
	case EvilEye::EvilEyeState::IDLE:
		break;
	//case EvilEyeState::HOVERING:
	//{
	//	Vector2 toTarget = hoverTarget - getPosition();

	//	if (Vector2Length(toTarget) < 20.f)
	//	{
	//		float angle = getRandomInt(data.rng, 0, 359) * DEG2RAD;

	//		hoverTarget =
	//		{
	//			data.player.getPosition().x + cosf(angle) * HOVER_RANGE,
	//			data.player.getPosition().y + sinf(angle) * HOVER_RANGE
	//		};

	//		toTarget = hoverTarget - getPosition();
	//	}

	//	moveDirection = toTarget;
	//	moveSpeed = 3.f;
	//	break;
	//}
	case EvilEyeState::HOVERING:
	{
		moveDirection = hoverTarget - getPosition();
		moveSpeed = 3.f;
		break;
	}
	case EvilEyeState::POSITION_FOR_DASH:
	{
		moveDirection = dashPosition - getPosition();
		moveSpeed = 4.f;
		break;
	}
	case EvilEye::EvilEyeState::DASH_WINDUP:
	{
		moveSpeed = 0.f;
		break;
	}
	case EvilEye::EvilEyeState::DASH:
	{
		//physics.velocity *= Vector2{ 1.5f,1.5f };
		moveDirection = dashDirection;
		moveSpeed = 12.f;
		break;
	}
	case EvilEyeState::DASH_RECOVER:
	{
		moveDirection = dashDirection;
		moveSpeed = 2.f;
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

	moveDirection = Vector2Normalize(moveDirection);

	if (moveSpeed != 0.f && moveDirection != Vector2{ 0,0 })
		physics.transform.pos += (moveDirection * moveSpeed * deltaTime);

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
	case EvilEye::EvilEyeState::IDLE:
		break;
	//case EvilEyeState::HOVERING:
	//{
	//	float angle = getRandomInt(data.rng, 0, 359) * DEG2RAD;

	//	hoverTarget =
	//	{
	//		data.player.getPosition().x + cosf(angle) * HOVER_RANGE,
	//		data.player.getPosition().y + sinf(angle) * HOVER_RANGE
	//	};

	//	break;
	//}
	case EvilEyeState::HOVERING:
	{
		hoverTarget =
		{
			data.player.getPosition().x,
			data.player.getPosition().y - HOVER_RANGE
		};

		break;
	}
	case EvilEyeState::POSITION_FOR_DASH:
	{
		Vector2 toPlayer = Vector2Normalize(data.player.getPosition() - getPosition());

		dashPosition =
		{
			data.player.getPosition().x - toPlayer.x * DASH_SIDE_OFFSET,
			data.player.getPosition().y - toPlayer.y * DASH_SIDE_OFFSET
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
