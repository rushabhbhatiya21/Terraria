#pragma once
#include <raylib.h>
#include "../items/item.h"

struct Entity;
struct AssetManager;
struct EntityUpdateData;

struct SwingAttack
{
	ItemId heldItem;
	Entity* owner;

	SwingAttack(ItemId heldItem, Entity* owner)
	{
		this->heldItem = heldItem;
		this->owner = owner;
	}

	float swingTimer = 0.f;
	float attackDuration = 0.f;

	// for weapon swing animation
	float weaponLength = 1.5f;
	Vector2 weaponBase = {};
	Vector2 weaponTip = {};

	bool isPlayingAttackAnimation = false;

	void render(AssetManager& assetManager);
	void update(float deltaTime);

};