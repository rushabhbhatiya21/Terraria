#pragma once
#include <entity.h>
#include <entityAnimation.h>

struct DesertSlime: public Entity
{
	DesertSlime()
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

	void dropLoot(EntityHolder& entityHolder, int type) override;

	Json formatToJson() override;

	bool loadFromJson(Json& j) override;

	void setColliderSize()
	{
		physics.transform.w = 1.f;
		physics.transform.h = 1.f;
	}

	int getEntityType() { return EntityType_DesertSlime; }

	float getMaxLife() { return 15; }

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