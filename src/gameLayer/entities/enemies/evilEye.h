#pragma once
#include "enemy.h"

struct EvilEye : public Enemy
{
	EvilEye()
	{
		shouldApplyGravity = false;
		shouldResolveConstraints = false;

		setColliderSize();
		life = getMaxLife();
	}

	enum class EvilEyeState
	{
		IDLE = 0,
		HOVERING,
		POSITION_FOR_DASH,
		DASH_WINDUP,
		DASH,
		DASH_RECOVER,
		DEAD
	};

	enum class EvilEyePhase
	{
		ONE,
		TWO
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

	float moveSpeed = 0.f;
	float rotation = 0.f;
	
	float stateChangeTimer = 0.f;

	static constexpr float HOVER_RANGE     = 8;
	static constexpr float DASH_INIT_RANGE = 12.f;

	static constexpr float DASH_WINDUP_TIME  = 0.5f;
	static constexpr float DASH_TIME         = 0.7f;
	static constexpr float DASH_RECOVER_TIME = 0.25f;

	static constexpr float DASH_SIDE_OFFSET = 8.f;
	static constexpr float DASH_HEIGHT_OFFSET = 0.f;
	static constexpr float DASH_POSITION_EPSILON = 0.5f;

	static constexpr int PHASE_ONE_CELL_SIZE_X = 111;
	static constexpr int PHASE_ONE_CELL_SIZE_Y = 163;

	static constexpr int PHASE_TWO_CELL_SIZE_X = 111;
	static constexpr int PHASE_TWO_CELL_SIZE_Y = 147;

	EvilEyeState currentState = EvilEyeState::HOVERING;
	EvilEyeState previousState = currentState;

	EvilEyePhase currentPhase = EvilEyePhase::ONE;

	int getEnemyType() override { return EnemyType_EvilEye; }

	float getMaxLife() override { return 100; }

	void drawSprite(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	void enterState(EvilEyeState newState, EntityUpdateData& entityUpdateData);

	void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;
};