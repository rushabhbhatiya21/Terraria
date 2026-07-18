#pragma once
#include <vector>
#include <random>
#include <math/vec2.h>
#include <items/item.h>
#include <bfs.h>

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
//	Engine::Vec2 start;
//	Engine::Vec2 end;
//
//    Engine::Vec2 position = {};
//    Engine::Vec2 direction = { 0,0 };
//
//
//	Engine::Vec2 mousePosition = { 0,0 };
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

	Engine::Vec2 mousePosition{};

	Engine::Vec2 position{};
	Engine::Vec2 direction{};

	// for weapon swing animation
	Engine::Vec2 weaponBase{};
	Engine::Vec2 weaponTip{};
	float   weaponLength = 1.5f; // add this in weaponData

	void render(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector);
	void updateAnimation(float deltaTime);
	void startSwing(Entity& entity, Engine::Vec2 mousePosition);
	void updateSwings(float deltaTime, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng);
	bool checkForHits(Enemy& enemy) const;

	void onHitEnemy(Enemy* enemy);
	void onHitBlock(int power, Engine::Vec2i blockPos, Block& b, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng);
	void onHitTree(Engine::Vec2i blockPos, GameMap& gameMap, EntityHolder& entityHolder);
	bool damageBlock(int power, const Engine::Vec2i& blockPos, Block& block, std::vector<Particle>& particles, std::ranlux24_base& rng);
	void destroyBlock(const Engine::Vec2i& blockPos, Block& block, GameMap& gameMap, EntityHolder& entityHolder);
};

//extern std::vector<Swing> swings;