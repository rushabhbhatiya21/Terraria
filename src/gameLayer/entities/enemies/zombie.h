#pragma once
#include "enemy.h"

struct Zombie : public Enemy
{
	Zombie()
	{
		setColliderSize();
	}

	enum class Zombie_State
	{
		IDLE = 0,
		PATROL,         // 1
		CHASING,        // 2
		ATTACK_WINDUP,  // 3
		ATTACK_RECOVER, // 4
		HURT,           // 5
		DEAD            // 6
	};

	Zombie_State currentState = Zombie_State::IDLE;
	Zombie_State previouseState = Zombie_State::IDLE;

	void drawSprite(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override;

	//void enterState(int newState, EntityUpdateData& entityUpdateData);

	bool shouldStepUp(Vector2 playerPos, GameMap& gameMap);

	bool isOnLedge(GameMap& gameMap);

	void doAttack(Player* player);

	void enterState(Zombie_State newState);

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

	virtual void onHit() override;

	// Zombie.h — tweak these freely
	bool isMoving = false;
	bool isMovingLeft = false;
	static constexpr float SIGHT_RANGE = 10.f;
	static constexpr float FORGET_RANGE = 15.f;
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
	static constexpr int ANIM_ATTACK = 2;
	static constexpr int ANIM_HURT = 3;

	float changeStateTimer = 3.f;
	float moveSpeed = 0.f; // also represents direction
};