#pragma once
#include "zombie.h"
#include <entities/droppedItem.h>
#include <items/blocks.h>
#include <gameMap.h>
#include <assets/assetManager.h>
#include <helper.h>
#include <entityHolder.h>
#include <player.h>
#include <combat/combatSystem.h>
#include <ui/popupText.h>
#include <shake.h>
#include <rendering/sprite.h>
#include <rendering/IRenderCollector.h>

//using Engine::AssetManager;

void Zombie::drawSprite(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	Transform2D zombieSprite = physics.transform;
	zombieSprite.w = 1;
	zombieSprite.h = 2;

	zombieSprite.pos.y -= (zombieSprite.h - physics.transform.h) / 2;

	auto aabb = getRectangleForEntity(zombieSprite, 1, 2);

	//DrawTexturePro(
	//	assetManager.zombie,
	//	getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft),
	//	aabb, // dest
	//	{ 0, 0 }, // origin top-left corner
	//	0.f, // rotation
	//	WHITE // tint
	//);

	Engine::Sprite sprite
	{
		&assetManager.zombie,
		&assetManager.defaultShader,
		getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft),
		aabb,
		{ 0,0 },
		0.f,
		Engine::White
	};
	collector.submitSprite(sprite);
}

bool Zombie::update(float deltaTime, EntityUpdateData& data)
{
	if (life <= 0 && isAlive) return false;

	// set variable for flipping sprite
	if (moveSpeed >= 0)
	{
		animations.movingLeft = false;
	}
	else
	{
		animations.movingLeft = true;
	}

	// --- TIMERS ---
	if (changeStateTimer   > 0)   changeStateTimer -= deltaTime;
	if (attackWindUpTimer  > 0)   attackWindUpTimer -= deltaTime;
	if (attackRecoverTimer > 0)   attackRecoverTimer -= deltaTime;
	if (hurtTimer          > 0)   hurtTimer -= deltaTime;

	Engine::Vec2 dirVector = data.player.getPosition() - getPosition();
	float dist = Engine::Vec2Length(dirVector);

	// exit chasing
	if (dist > FORGET_RANGE && currentState == Zombie_State::CHASING)
	{
		// printf("exiting state chasing.\n");
		enterState(Zombie_State::IDLE);
	}

	// exit attack
	if (currentState == Zombie_State::ATTACK_RECOVER && attackRecoverTimer <= 0)
	{
		// printf("exiting state attack.\n");
		enterState(Zombie_State::CHASING);
	}

	// exit hurt
	if (currentState == Zombie_State::HURT && hurtTimer <= 0)
	{
		// printf("exiting state hurt.\n");
		enterState(Zombie_State::CHASING);
	}

	// patrol phase
	if (changeStateTimer <= 0)
	{
		if (currentState == Zombie_State::IDLE)
		{
			enterState(Zombie_State::PATROL);
		}
		else if (currentState == Zombie_State::PATROL)
		{
			// printf("exiting state patrol.\n");
			enterState(Zombie_State::IDLE);
		}
	}

	// transitions
	// chase
	if (dist > ATTACK_RANGE &&
		dist <= SIGHT_RANGE &&
		currentState != Zombie_State::ATTACK_WINDUP &&
		currentState != Zombie_State::ATTACK_RECOVER &&
		currentState != Zombie_State::HURT)
	{
		enterState(Zombie_State::CHASING);
	}

	// attack windup
	if (dist <= ATTACK_RANGE &&
		currentState != Zombie_State::ATTACK_RECOVER &&
		currentState != Zombie_State::HURT)
	{
		enterState(Zombie_State::ATTACK_WINDUP);
	}

	// attack recover
	if (currentState == Zombie_State::ATTACK_WINDUP && attackWindUpTimer <= 0)
	{
		// actual hit, player damage
		if (dist <= ATTACK_RANGE)
		{
			// printf("entering state attack.\n");
			doAttack(&data.player);
		}

		enterState(Zombie_State::ATTACK_RECOVER);
	}

	// movement (act) phase
	switch (currentState)
	{
	case Zombie_State::IDLE:
	{
		moveSpeed = 0;
		break;
	}
	case Zombie_State::PATROL:
	{
		moveSpeed = WANDER_SPEED * (isMovingLeft ? -1.f : 1.f);
		break;
	}
	case Zombie_State::CHASING:
	{
		moveSpeed = CHASE_SPEED * (dirVector.x < 0 ? -1.f : 1.f);

		if (physics.downTouch && shouldStepUp(data.player.getPosition(), data.gameMap))
		{
			physics.jump(7);
		}
		break;
	}
	case Zombie_State::ATTACK_WINDUP:
	{
		moveSpeed = 0;
		animations.setAnimation(ANIM_ATTACK);
		break;
	}
	case Zombie_State::ATTACK_RECOVER:
	{
		moveSpeed = 0;
		//animations.setAnimation(ANIM_ATTACK);
		break;
	}
	case Zombie_State::HURT:
	{
		moveSpeed = 0;
		//animations.setAnimation(ANIM_HURT);
		break;
	}
	case Zombie_State::DEAD:
	{
		// cleanup will happen in update, not sure what to do now
		moveSpeed = 0;
		break;
	}
	default:
		break;
	}

	if (moveSpeed != 0.f)
	{
		getPosition().x += moveSpeed * deltaTime;
	}

	// --- ANIMATION ---
	if (!physics.downTouch)
		animations.setAnimation(ANIM_JUMP);
	else if (moveSpeed == 0.f)
		animations.setAnimation(ANIM_IDLE);
	// todo: need to test this 2 animations
	// todo: a bug where if zombie is attacking, it turns to 1 side
	if (currentState == Zombie_State::ATTACK_WINDUP || currentState == Zombie_State::ATTACK_RECOVER)
		animations.setAnimation(ANIM_ATTACK);
	else if (currentState == Zombie_State::HURT)
		animations.setAnimation(ANIM_HURT);
	else
		animations.setAnimation(ANIM_WALK);

	animations.update(deltaTime, 0.08f, 7);

	return true;
}

void Zombie::enterState(Zombie_State newState)
{
	if (currentState == newState)
		return;

	previouseState = currentState;
	currentState = newState;

	switch (currentState)
	{
	case Zombie_State::IDLE:
		changeStateTimer = 1.f;
		break;
	case Zombie_State::PATROL:
		// printf("entering state patrol.\n");
		changeStateTimer = 3.f;
		isMovingLeft = !isMovingLeft;
		break;
	case Zombie_State::CHASING:
		// printf("entering state chasing.\n");
		hurtTimer = 0.f;
		attackWindUpTimer = 0.f;
		attackRecoverTimer = 0.f;
		break;
	case Zombie_State::ATTACK_WINDUP:
		// printf("entering state attack windup.\n");
		attackWindUpTimer = .4f;
		break;
	case Zombie_State::ATTACK_RECOVER:
		// printf("entering state attack recover.\n");
		attackRecoverTimer = .6f;
		break;
	case Zombie_State::HURT:
		// printf("entering state hurt.\n");
		hurtTimer = .15f;
		break;
	default:
		break;
	}
}

void Zombie::doAttack(Player* player)
{
	// implement attack here
	DamageInfo info;
	info.attacker = this;
	info.hitDirection = Engine::Vec2Normalize(physics.velocity);

	CombatSystem::applyDamage(player, info);

	return;
}

void Zombie::onHit()
{
	Enemy::onHit();
	enterState(Zombie_State::HURT);
}

void Zombie::dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder)
{
	int dropsCount = 0;
	//bool legendary = false;
	//bool epic      = false;
	//bool rare      = false;
	//bool common    = false;

	if (getRandomChance(rng, .1f))
		dropsCount = 10;
	else if (getRandomChance(rng, .2f))
		dropsCount = 6;
	else if (getRandomChance(rng, .4f))
		dropsCount = 4;
	else
		dropsCount = 2;

	bool isLeft = false;
	for (int i = 0; i < dropsCount; i++)
	{
		DroppedItem droppedItem;
		droppedItem.teleport(getPosition());

		// make it drop rarer chests with low chance
		droppedItem.itemType = Items::slime;
		float x = getRandomFloat(rng, -3.f, 3.f);
		if ((isLeft && x < 0) || (!isLeft && x > 0)) x *= -1.f;
		isLeft = !isLeft;
		droppedItem.physics.velocity.x = x;
		droppedItem.physics.velocity.y = -3.f;

		auto id = entityHolder.idHolder.getEntityIdAndIncreament();
		entityHolder.droppedItems.push_back(&droppedItem);
		entityHolder.entities[id] = std::make_unique<DroppedItem>(droppedItem);
	}
}

bool Zombie::shouldStepUp(Engine::Vec2 playerPos, GameMap& gameMap)
{
	Engine::Vec2 zTotPlayerDirection = playerPos - getPosition();

	int nextX = int(getPosition().x) + 1;
	int prevX = int(getPosition().x) - 1;

	auto* bNext = gameMap.getBlockSafe(nextX, (int)getPosition().y);
	auto* bPrev = gameMap.getBlockSafe(prevX, (int)getPosition().y);

	auto* bNextDef = getItem(bNext->type);
	auto* bPrevDef = getItem(bPrev->type);

	if (bNext && zTotPlayerDirection.x >= 0 && bNextDef && bNextDef->block.isCollidable())
		return true;

	if (bPrev && zTotPlayerDirection.x < 0 && bPrevDef && bPrevDef->block.isCollidable())
		return true;

	return false;
}

bool Zombie::isOnLedge(GameMap& gameMap)
{
	if (!physics.downTouch)
		return false;

	int belowY = int(getPosition().y) + 1;

	if (moveSpeed > 0.f) // moving right, check right side
	{
		int nextX = int(getPosition().x) + 1;
		auto bNext = gameMap.getBlockSafe(nextX, belowY);

		auto* bNextDef = getItem(bNext->type);

		return !bNext || !bNextDef || !bNextDef->block.isCollidable();
	}
	else if (moveSpeed < 0.f) // moving left, check left side
	{
		int prevX = int(getPosition().x) - 1;
		auto bPrev = gameMap.getBlockSafe(prevX, belowY);
		auto* bPrevDef = getItem(bPrev->type);

		return !bPrev || !bPrevDef || !bPrevDef->block.isCollidable();
	}

	return false; // not moving, not on edge
}

Json Zombie::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo zombie state

	return j;
}

bool Zombie::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}