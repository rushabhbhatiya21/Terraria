#pragma once
#include "enemy.h"

struct Zombie : public Enemy
{
	Zombie()
	{
		setColliderSize();
		life = getMaxLife();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	void dropLoot(EntityHolder& entityHolder, int type) override;

	void enterState(int newState, EntityUpdateData& entityUpdateData);

	bool shouldStepUp(Vector2 playerPos, GameMap& gameMap);

	bool isOnLedge(GameMap& gameMap);

	void doAttack(EntityUpdateData& entityUpdateData);

	//void enterState(int newState, EntityUpdateData& entityUpdateData) override;

	//bool shouldStepUp(Vector2 playerPos, GameMap& gameMap) override;

	//bool isOnLedge(GameMap& gameMap) override;

	//void doAttack(EntityUpdateData& entityUpdateData) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	int getEnemyType() override { return EnemyType_Zombie; }

	float getMaxLife() override { return 50; }

	// Zombie.h — tweak these freely
	static constexpr float SIGHT_RANGE = 15.f;
	static constexpr float ATTACK_RANGE = 1.5f;
	static constexpr float WANDER_SPEED = 1.5f;   // px/s
	static constexpr float CHASE_SPEED = 3.f;
	static constexpr float IDLE_INTERVAL = 2.f;    // seconds
	static constexpr float WANDER_INTERVAL = 3.f;
	static constexpr float ATTACK_COOLDOWN = 1.f;
	static constexpr float HURT_DURATION = 0.3f;

	static constexpr int ANIM_JUMP = 0;
	static constexpr int ANIM_IDLE = 0;
	static constexpr int ANIM_WALK = 1;
	static constexpr int ANIM_DEAD = 4;

	float changeStateTimer = 3.f;
	float moveSpeed = 0.f; // also represents direction
};