#pragma once
#include <physics.h>
#include <random>
#include <nlohmann/json.hpp>

struct Player;
struct AssetManager;
struct EntityHolder;
struct Inventory;

using Json = nlohmann::json;

enum EntityType
{
	EntityType_Player = 0,
	EntityType_DroppedItem,
	EntityType_Enemy
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
	bool isAlive = false;
	float life = 1;
	bool isFacingRight = true;
	float isRedTimer = 0.f;
	float hitStopTimer = 0;

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

	virtual void render(AssetManager& assetManager) = 0;

	virtual bool update(float deltaTime, EntityUpdateData& entityUpdateData) = 0;

	virtual int getEntityType() = 0;

	virtual void setColliderSize() = 0;

	virtual float getMaxLife() = 0;

	virtual void dropLoot(EntityHolder& entityHolder, int type) = 0;

	virtual void hit(float damage, Vector2 hitFromPosition) = 0;

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