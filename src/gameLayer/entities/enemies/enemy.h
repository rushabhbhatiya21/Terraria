#pragma once
#include <physics.h>
#include <randomStuff.h>
#include <entityAnimation.h>
#include <entity.h>

enum EnemyType
{
	EnemyType_Slime = 101,
	EnemyType_DesertSlime,
	EnemyType_Zombie,
	EnemyType_EvilEye
};

struct Enemy : public Entity
{
	EntityAnimation animations;

	float baseX = 0;
	int shaderLoc = -1;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	float flashTimer = 0;
	//float hurtTimer = 0;

	void renderHealthBar(AssetManager& assetManager);

	bool updateHealthBar(float deltatime);

	virtual void render(AssetManager& assetManager) override;

	virtual void drawSprite(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData& entityUpdateData) override = 0;

	virtual void enterState(int newState) {};

	virtual void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override = 0;

	//virtual void enterState(int newState, EntityUpdateData& entityUpdateData) = 0;

	//virtual bool shouldStepUp(Vector2 playerPos, GameMap& gameMap) = 0;

	//virtual bool isOnLedge(GameMap& gameMap) = 0;

	//virtual void doAttack(EntityUpdateData& entityUpdateData) = 0;

	virtual Json formatToJson() override = 0;

	virtual bool loadFromJson(Json& j) override = 0;

	int getEntityType() override { return EntityType_Enemy; }

	virtual int getEnemyType() = 0;

	virtual void onHit();

	float attackWindUpTimer = 0;
	float attackRecoverTimer = 0;

	//static float constexpr attackWindUpTime  = .4f;
	//static float constexpr attackRecoverTime = .6f;

	int currentState = 0;
	//int previouseState = 0;

	enum
	{
		STATE_WONDERING,
		STATE_CHASING,
		STATE_DEAD
	};
};