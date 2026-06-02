#pragma once
#include "enemy.h"

struct Slime : public Enemy
{
	Slime()
	{
		setColliderSize();
		life = getMaxLife();
	}

	void drawSprite(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData& entityUpdateData) override;

	void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.8f;
		physics.transform.h = 0.8f;
	}

	int getEnemyType() override { return EnemyType_Slime; }

	float getMaxLife() override { return 35; }

	//enum
	//{
	//	STATE_WONDERING = 0,
	//	STATE_CHASING,
	//	STATE_DEAD
	//};

	int currentState = STATE_WONDERING;
	float changeStateTimer = 1.f;
	float jumpTimer = 5.f;
	float moveSpeed = 0.f; // also represents direction
};