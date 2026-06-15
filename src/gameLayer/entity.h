#pragma once
#include <physics.h>
#include <random>
#include <nlohmann/json.hpp>

#include "inventory.h"
#include "equipmentInventory.h"
#include "combat/stats.h"
#include "entityAnimation.h"
#include "attackStyles/swingAttack.h"

struct Player;
struct GameMap;
struct EntityHolder;
struct AssetManager;

using Json = nlohmann::json;

enum EntityType
{
	EntityType_Player = 0,
	EntityType_DroppedItem,
	EntityType_Enemy,
	EntityType_Boss,
	//EntityType_Spawn,
	EntityType_Projectile
};

struct EntityUpdateData
{
	Player& player;
	std::ranlux24_base& rng;
	EntityHolder& entityHolder;
	Inventory& inventory;
	GameMap& gameMap;

	std::uint64_t ownId = 0;
};

struct Entity
{
	PhysicalEntity physics;

	EquipmentInventory equipments;
	EntityAnimation animations;

	EntityStats baseStats;
	EntityStats stats;

	Inventory inventory;

	ItemId heldItem = Items::air;
	SwingAttack swingStyle;

	bool isAlive = false;
	float life = 0.f;
	bool shouldApplyGravity = true;
	bool shouldResolveConstraints = true;

	float useTimer = 0;
	float flashTimer = 0.f;
	float blinkTimer = 0.f;
	float hitStopTimer = 0.f;
	float damageTaken = 0.f;
	float damageTakenHealthBarTimer = 0.f;
	float hurtTimer = 0.f;

	static constexpr float MAX_FORCE = 10.f;

	Entity()
	{
		isAlive = true;
	}

	Vector2& getPosition()
	{
		return physics.transform.pos;
	}

	void teleport(Vector2 pos)
	{
		physics.teleport(pos);
	}

	void knockback(Vector2 hitDirection, float finalKnockback)
	{
		float force = MAX_FORCE * (finalKnockback / 100.0f);
		physics.velocity += hitDirection * force;
	}

	virtual void render(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData& entityUpdateData) = 0;

	virtual int getEntityType() = 0;

	virtual void setColliderSize() = 0;

	virtual void dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder) {};

	virtual void onHit() {}

	virtual void recalculateStats() {};

	virtual Json formatToJson() = 0;
	virtual bool loadFromJson(Json& j) = 0;

	void addCommonEntityStuffToJson(Json& json)
	{
		json["physics"] = physics.formatToJson();
		json["life"] = life;
		json["entityType"] = getEntityType();
	}

	bool loadCommonEntityStuffFromJson(Json& json)
	{
		if (!json.contains("physics")) { return false; }
		
		auto j = json["physics"];

		if (!j.is_object()) { return false; }

		if (!physics.loadFromJson(j)) { return false; }

		if (json["life"].is_number())
		{
			life = json["life"];
		}

		return true;
	}
};