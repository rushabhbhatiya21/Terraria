#pragma once
#include <vector>
#include <random>
#include <raylib.h>
#include "../items/item.h"
#include "../bfs.h"

namespace Engine
{
	struct AssetManager;
	class IRenderCollector;
}

struct Block;
struct Enemy;
struct Entity;
struct GameMap;
struct Particle;
struct EntityHolder;
struct EntityUpdateData;

//struct Swing
//{
//    Entity* owner = nullptr;
//
//	ItemId itemId;
//
//	Vector2 start;
//	Vector2 end;
//
//    Vector2 position = {};
//    Vector2 direction = { 0,0 };
//
//
//	Vector2 mousePosition = { 0,0 };
//};

struct SwingAttack
{
	bool facingLeft = false;
	bool isPlayingAnimation = false;

	Entity* owner = nullptr;

	ItemId itemId = Items::air;

	float lifetime = 0.f;
	float swingTimer = 0.f;
	float animationDuration = 0.f;

	float currentSwingAngle = 0.f;

	Vector2 mousePosition{};

	Vector2 position{};
	Vector2 direction{};

	// for weapon swing animation
	Vector2 weaponBase{};
	Vector2 weaponTip{};
	float   weaponLength = 1.5f; // add this in weaponData

	void render(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector);
	void updateAnimation(float deltaTime);
	void startSwing(Entity& entity, Vector2 mousePosition);
	void updateSwings(float deltaTime, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng);
	bool checkForHits(Enemy& enemy) const;

	void onHitEnemy(Enemy* enemy);
	void onHitBlock(int power, Vector2i blockPos, Block& b, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng);
	void onHitTree(Vector2i blockPos, GameMap& gameMap, EntityHolder& entityHolder);
	bool damageBlock(int power, const Vector2i& blockPos, Block& block, std::vector<Particle>& particles, std::ranlux24_base& rng);
	void destroyBlock(const Vector2i& blockPos, Block& block, GameMap& gameMap, EntityHolder& entityHolder);
};

//extern std::vector<Swing> swings;