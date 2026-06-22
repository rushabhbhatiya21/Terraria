#include "swingAttack.h"
#include "../shake.h"
#include "../particles.h"
#include "../helper.h"
#include "../entity.h"
#include "../gameMap.h"
#include "../assetManager.h"
#include "../entityHolder.h"
#include "../items/item.h"
#include "../items/blocks.h"
#include "../ui/popupText.h"
#include "../entities/droppedItem.h"
#include "../combat/combatSystem.h"
#include "../entities/enemies/enemy.h"
#include <lighting.h>

void SwingAttack::render(AssetManager& assetManager)
{
	if (!owner) return;
	
	if (!isPlayingAnimation) return;

	Texture2D  texture = getTextureForItemType(itemId, assetManager);
	Rectangle  textureUVItem = getTextureCoordinatesForItemType(itemId);

	// player position
	auto pos = owner->physics.transform.getAABB();

	// approx. hand position
	Vector2 handPos{};
	handPos.x = pos.x + (facingLeft ? 1.5f : .5f);
	handPos.y = pos.y + 1.5f;

	// Weapon size in world units
	float weaponWidth = 1.0f;
	float weaponHeight = 1.0f;

	Rectangle destRect{};
	destRect.width = weaponWidth;
	destRect.height = weaponHeight;

	// Pivot inside the weapon sprite
	Vector2 origin{};

	if (facingLeft)
	{
		textureUVItem = flipTextureAtlasX(textureUVItem);
	}

	float bottomArcOffset = .3f;
	origin.x = facingLeft ? (weaponWidth + bottomArcOffset) : -bottomArcOffset;
	origin.y = weaponHeight;
	destRect.x = handPos.x - origin.x;
	destRect.y = handPos.y - origin.y;

	DrawTexturePro(texture, textureUVItem, destRect, origin, currentSwingAngle, WHITE);
}

void SwingAttack::startSwing(Entity& owner, Vector2 mousePosition)
{
	this->facingLeft = owner.animations.movingLeft;
	this->itemId = owner.heldItem;

	this->owner = &owner;
	this->direction = facingLeft
		? Vector2{ -1.f, 0.f }
	    : Vector2{  1.f, 0.f };

	// swing exists briefly
	this->lifetime = owner.useTimer;
	this->swingTimer = owner.useTimer;
	this->animationDuration = owner.useTimer;

	// offset swing in facing direction
	this->position = owner.getPosition();
	this->position += direction * owner.stats.offensive.range; // not sure if range is set in item.cpp

	this->mousePosition = mousePosition;
}

void SwingAttack::updateAnimation(float deltaTime)
{
	swingTimer = std::max(0.f, swingTimer - deltaTime);

	if (swingTimer <= 0)
	{
		owner = nullptr;
		swingTimer = 0;
		animationDuration = 0;
		currentSwingAngle = 0;
		isPlayingAnimation = false;
		return;
	}

	if (!owner) return;

	isPlayingAnimation = true;

	// swing logic
	weaponBase = owner->getPosition();

	float t = 1.f - (swingTimer / animationDuration);
	t = Clamp(t, 0.f, 1.f);
	t = t * t * (3.f - 2.f * t);

	float startAngle, endAngle;

	if (!facingLeft) { startAngle = -210.f; endAngle = 90.f; }
	else { startAngle = 210.f; endAngle = -90.f; }

	currentSwingAngle = Lerp(startAngle, endAngle, t);

	float radians = DEG2RAD * currentSwingAngle;
	weaponTip.x = weaponBase.x + cosf(radians) * weaponLength;
	weaponTip.y = weaponBase.y + sinf(radians) * weaponLength;
}

void SwingAttack::updateSwings(float deltaTime, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng)
{
	lifetime = std::max(0.f, lifetime - deltaTime);

	// remove expired attacks
	if (lifetime <= 0.f)
	{
		lifetime = 0;
		return;
	}

	if (!owner)
	{
		lifetime = 0;
		swingTimer = 0;
		return;
	}

	// collision against enemies
	for (Enemy* enemy : entityHolder.enemies)
	{
		if (!enemy->isAlive) continue;

		if (checkForHits(*enemy))
		{
			onHitEnemy(enemy);
		}
	}

	auto* item = getItem(itemId);
	if (!item)
	{
		lifetime = 0;
		swingTimer = 0;
		return;
	}

	if (item->category != ItemCategory::TOOL) return;

	Vector2i blockPos{
		(int)mousePosition.x,
		(int)mousePosition.y
	};

	auto* b = gameMap.getBlockSafe(blockPos.x, blockPos.y);
	auto* bDef = getItem(b->type);

	if (!b || !bDef || !bDef->block.isCollidable()) return;

	float dist = Vector2Distance(blockPos.toVector2(), owner->getPosition());
	int toolRange = item->tool.tool.range;

	if (dist <= toolRange)
	{
		// onHitBlock
		onHitBlock(item->tool.tool.miningPower, blockPos, *b, gameMap, entityHolder, particles, rng);

		if (b->hp <= 0 && b->type == Items::woodLog)
		{
			// onHitTree
			onHitTree(blockPos, gameMap, entityHolder);
		}
		return;
	}
}

bool SwingAttack::checkForHits(Enemy& enemy) const
{
	//DrawLineEx(base, tip, .05f, GREEN);

	for (float t = 0; t <= 1; t += 0.2f)
	{
		Vector2 p = Vector2Lerp(weaponBase, weaponTip, t);

		//DrawCircleV(p, .1f, YELLOW);

		if (CheckCollisionPointRec(p, enemy.physics.transform.getAABB()))
		{
			return true;
		}
	}
	return false;
}

void SwingAttack::onHitEnemy(Enemy* enemy)
{
	DamageInfo info;
	info.attacker = owner;
	info.hitDirection = facingLeft
		? Vector2{ -1.f, 0.f }
	    : Vector2{ 1.f, 0.f };;

	// this will trigger onHit for each entity
	CombatSystem::applyDamage(enemy, info);

	// prevent multi-hit from same swing
	lifetime = 0.f;
	return;
}

void SwingAttack::onHitBlock(int power, Vector2i blockPos, Block& b, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng)
{
	auto brokenType = b.type;
	if (damageBlock(power, blockPos, b, particles, rng))
	{
		destroyBlock(blockPos, b, gameMap, entityHolder);
	}
	lifetime = 0;
	return;
}

void SwingAttack::onHitTree(Vector2i blockPos, GameMap& gameMap, EntityHolder& entityHolder)
{
	auto res = bfs(blockPos, gameMap);

	if (!res.empty())
	{
		for (auto& block : res)
		{
			auto* tb = gameMap.getBlockSafe(block.x, block.y);
			if (!tb) continue;
			destroyBlock(block, *tb, gameMap, entityHolder);
		}
	}
}

bool SwingAttack::damageBlock(int power, const Vector2i& blockPos, Block& block, std::vector<Particle>& particles, std::ranlux24_base& rng)
{
	// make particles global like popupText or camShake
	triggerShake(blockPos.x, blockPos.y);
	spawnParticles({ (float)blockPos.x, (float)blockPos.y }, rng, block.type, 10);

	spawnPopupText(
		blockPos.toVector2(),
		Vector2{ 0, .1f },
		std::to_string(power),
		1,
		.2f,
		-1.f,
		WHITE,
		false
	);

	int newHp = (int)block.hp - power;

	if (newHp <= 0)
		return true;

	block.hp = uint16_t(newHp);
	return false;
}

void SwingAttack::destroyBlock(const Vector2i& blockPos, Block& block, GameMap& gameMap, EntityHolder& entityHolder)
{
	if (block.type == Items::air)
		return;

	// store light BEFORE modifying block
	uint8_t oldLight = block.light;

	// drop item
	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	auto item = std::make_unique<DroppedItem>();
	item->teleport(blockPos.center());
	item->itemType = block.type;
	item->physics.velocity.y = -3.f;

	DroppedItem* itemPtr = item.get();
	entityHolder.entities[id] = std::move(item);
	entityHolder.droppedItems.push_back(itemPtr);

	gameMap.setBlock(blockPos.x, blockPos.y, Items::air, true);
	//recalculateLight(gameMap);
}
