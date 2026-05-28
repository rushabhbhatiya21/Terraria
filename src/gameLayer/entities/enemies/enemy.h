#pragma once
#include <physics.h>
#include <randomStuff.h>
#include <entityAnimation.h>
#include <entity.h>

enum EnemyType
{
	EnemyType_Slime = 101,
	EnemyType_DesertSlime,
	EnemyType_EvilEye,
	EnemyType_Zombie
};

struct Enemy : public Entity
{
	//Enemy()
	//{
	//	isAlive = true;
	//}

	EntityAnimation animations;

	float baseX = 0;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	float flashTimer = 0;
	float hurtTimer = 0;

	void renderHealthBar(AssetManager& assetManager);

	bool updateHealthBar(float deltatime);

	virtual void render(AssetManager& assetManager) override;

	virtual void drawSprite(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData& entityUpdateData) override = 0;

	virtual void dropLoot(EntityHolder& entityHolder, int type) override = 0;

	//virtual void enterState(int newState, EntityUpdateData& entityUpdateData) = 0;

	//virtual bool shouldStepUp(Vector2 playerPos, GameMap& gameMap) = 0;

	//virtual bool isOnLedge(GameMap& gameMap) = 0;

	//virtual void doAttack(EntityUpdateData& entityUpdateData) = 0;

	virtual Json formatToJson() override = 0;

	virtual bool loadFromJson(Json& j) override = 0;

	int getEntityType() override { return EntityType_Enemy; }

	virtual int getEnemyType() = 0;

	virtual float getMaxLife() override = 0;

	virtual void onHit();

	int currentState = STATE_IDLE;

	enum
	{
		STATE_IDLE = 0,
		STATE_WONDERING,
		STATE_CHASING,
		STATE_ATTACK,
		STATE_HURT,
		STATE_DEAD
	};
};