#pragma once
#include "enemy.h"
#include <combat/stats.h>

struct EvilEyeServant : public Enemy
{
	EvilEyeServant()
	{
		baseStats = makeServantEvilEyeStats();
		stats = baseStats;
		life = (float)stats.defensive.maxHealth;

		shouldApplyGravity = false;
		shouldResolveConstraints = false;

		setColliderSize();
	}

	void setColliderSize()
	{
		physics.transform.w = 1.f;
		physics.transform.h = 1.f;
	}
	
	enum class ServantState
	{
		CHASING = 0,
		DEAD_ANIM,
		DEAD
	};

	float rotation = 0;
	float moveSpeed = 1.f;
	float spriteScale = .5f;
	
	float delayTimer = 0;
	float stateChangeTimer = 0;

	bool isFlashTimerSet = false;
	bool isColliding = false;
	bool wasColliding = false;

	Vector2 moveDirection = {};

	ServantState currentState = ServantState::CHASING;
	ServantState previousState = currentState;

	static constexpr float DEAD_TIMER = 1.5f;

	int getEntityType() override { return EntityType::EntityType_Enemy; }

	int getEnemyType() override { return EnemyType_EvilEyeSpawn; }

	void drawSprite(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& data) override;

	void enterState(ServantState newState);

	void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;
};