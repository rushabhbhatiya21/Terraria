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

	float damageTaken = 0;
	float hurtTimer = 0;
	float baseX = 0;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void renderHealthBar(AssetManager& assetManager);

	bool updateHealthBar(float deltatime);

	virtual void render(AssetManager& assetManager) override = 0;

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

	void knockback(float damage, Vector2 hitFromPosition, float knockbackForce = 3.f)
	{
		// Push away from whoever hit us
		float direction = (getPosition().x >= hitFromPosition.x) ? 1.f : -1.f;

		physics.velocity.x = direction * knockbackForce;
		physics.velocity.y = -3.f; // small upward bump, feels more impactful
	}

	void hit(float damage, Vector2 hitFromPosition) override
	{
		isRedTimer = hitStopTimer;
		life -= damage;
		damageTaken = damage;
		hurtTimer = 1;

		if (life <= 0) return;
		knockback(damage, hitFromPosition);

		// Force into hurt state — you'll call enterState from the subclass
		currentState = STATE_HURT;
	}

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