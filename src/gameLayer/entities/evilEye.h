#pragma once
#include <entity.h>
#include <entityAnimation.h>

struct EvilEye : public Entity
{
	EvilEye()
	{
		physics.transform.w = 1.f;
		physics.transform.h = 1.f;

		life = getMaxLife();
		baseY = getPosition().y;
	}

	//EntityAnimation animation;

	float baseY = 0;

	Vector2& getPosition()
	{
		return physics.getPosition();
	}

	void render(AssetManager& assetManager) override;

	bool update(float deltaTime, EntityUpdateData entityUpdateData) override;

	int getEntityType() { return EntityType_EvilEye; }

	float getMaxLife() { return 5; };

	enum
	{
		STATE_WONDERING = 0,
		STATE_CHASING
	};

	int currentState = STATE_WONDERING;
	float time = 0.f;
	float changeStateTimer = 1.f;
	float moveTimer = 5.f;
	float moveSpeed = 1.f; // also represents direction
	float animationSpeed = 2.f;

};