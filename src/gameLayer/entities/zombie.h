#pragma once
#include <physics.h>
#include <randomStuff.h>
#include <entityAnimation.h>
#include <entity.h>


struct Zombie : public Entity
{
	Zombie()
	{
		setColliderSize();

		life = getMaxLife();
	}

	EntityAnimation animation;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 0.9f;
		physics.transform.h = 1.8f;
	}

	int getEntityType() { return EntityType_Zombie; }

	float getMaxLife() { return 10; }

	enum
	{
		STATE_WONDERING = 0,
		STATE_CHASING
	};

	int currentState = STATE_WONDERING;
	float changeStateTimer = 1.f;
	float jumpTimer = 5.f;
	float moveSpeed = 0.f; // also represents direction
};