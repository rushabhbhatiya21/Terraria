#pragma once
#include "enemy.h"
#include <combat/stats.h>

struct PhaseData
{
	float hoverRange;
	float dashInitRange;

	float dashWindupTime;
	float dashTime;
	float dashRecoverTime;

	float dashSideOffset;
	float dashHeightOffset;
	float dashPositionEpsilon;
	int   maxDashCount;

	float hoverSpeed;
	float positionSpeed;
	float dashSpeed;
	float recoverSpeed;

	float moveAcceleration;

	int cellSizeX;
	int cellSizeY;
};

static constexpr PhaseData PHASE_1 =
{
	8.f,    // hoverRange
	12.f,   // dashInitRange

	0.5f,   // dashWindupTime
	0.7f,   // dashTime
	0.25f,  // dashRecoverTime

	8.f,    // dashSideOffset
	0.f,    // dashHeightOffset
	0.5f,   // dashPositionEpsilon
	3,      // maxDashCount

	2.f,    // hoverSpeed
	3.f,    // positionSpeed
	8.f,    // dashSpeed
	2.f,    // recoverSpeed

	5.f,    // moveAcceleration

	111,    // cellSizeX
	163     // cellSizeY
};

static constexpr PhaseData PHASE_2 =
{
	6.f,    // hoverRange
	10.f,   // dashInitRange

	0.25f,  // dashWindupTime
	0.6f,   // dashTime
	0.1f,   // dashRecoverTime

	6.f,    // dashSideOffset
	0.f,    // dashHeightOffset
	0.5f,   // dashPositionEpsilon
	5,      // maxDashCount

	4.f,    // hoverSpeed
	6.f,    // positionSpeed
	16.f,   // dashSpeed
	3.f,    // recoverSpeed

	8.f,    // moveAcceleration

	111,    // cellSizeX
	147     // cellSizeY
};

struct EvilEye : public Enemy
{
	EvilEye()
	{
		stats = makeEvilEyeStats();
		life = (float)stats.maxHealth;

		shouldApplyGravity = false;
		shouldResolveConstraints = false;

		setColliderSize();
	}

	enum class EvilEyeState
	{
		HOVERING = 0,
		POSITION_FOR_DASH,
		DASH_WINDUP,
		DASH,
		DASH_RECOVER,
		DEAD
	};

	enum class EvilEyePhase
	{
		ONE,
		TWO,
		DEAD
	};

	void setColliderSize()
	{
		physics.transform.w = 2.5f;
		physics.transform.h = 2.5f;
	}

	Vector2 moveDirection   = {};
	Vector2 lockedPlayerPos = {};
	Vector2 hoverTarget = {};
	Vector2 dashDirection{};
	Vector2 dashPosition{};

	bool wasColliding = false;
	bool isColliding  = false;

	int dashCounter = 0;

	float moveSpeed = 0.f;
	float rotation = 0.f;
	
	float stateChangeTimer = 0.f;

	EvilEyeState currentState = EvilEyeState::HOVERING;
	EvilEyeState previousState = currentState;

	EvilEyePhase currentPhase = EvilEyePhase::ONE;
	const PhaseData* currentPhaseData = &PHASE_1;

	int getEnemyType() override { return EnemyType_EvilEye; }

	void drawSprite(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	void enterState(EvilEyeState newState, EntityUpdateData& entityUpdateData);

	void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;
};