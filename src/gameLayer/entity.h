#pragma once
#include <physics.h>
#include <random>
#include <nlohmann/json.hpp>

struct Player;
struct AssetManager;
struct EntityHolder;
struct Inventory;
struct EquipmentInventory;

using Json = nlohmann::json;

enum EntityType
{
	EntityType_Player = 0,
	EntityType_DroppedItem,
	EntityType_Enemy,
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

	bool isAlive = false;
	float life = 1;
	bool shouldApplyGravity = true;

	// not sure to keep or not
	bool isFacingRight = true;

	float attackDuration = 0.f;
	float swingTimer = 0.f;
	float useTimer = 0.f;
	float hitStopTimer = 0;
	float damageTaken = 0;

	// for weapon swing animation
	float weaponLength = 1.5f;
	Vector2 weaponBase = {};
	Vector2 weaponTip = {};

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

	void knockback(Vector2 hitFromPosition, float knockbackForce = 3.f)
	{
		// Push away from whoever hit us
		float direction = (getPosition().x >= hitFromPosition.x) ? 1.f : -1.f;

		physics.velocity.x = direction * knockbackForce;
		physics.velocity.y = -3.f; // small upward bump, feels more impactful
	}

	virtual void render(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData& entityUpdateData) = 0;

	virtual int getEntityType() = 0;

	virtual void setColliderSize() = 0;

	virtual float getMaxLife() = 0;

	virtual void dropLoot(EntityHolder& entityHolder, int type) {};

	virtual void onHit() {}

	//virtual void hit(float damage, Vector2 hitFromPosition) = 0;

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