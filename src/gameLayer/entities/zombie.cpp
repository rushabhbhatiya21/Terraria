#pragma once
#include "zombie.h"
#include "droppedItem.h"
#include <blocks.h>
#include <gameMap.h>
#include <assetManager.h>
#include <helper.h>
#include <entityHolder.h>

void Zombie::render(AssetManager& assetManager)
{
	Transform2D zombieSprite = physics.transform;
	zombieSprite.w = 1;
	zombieSprite.h = 2;

	zombieSprite.pos.y -= (zombieSprite.h - physics.transform.h) / 2;

	auto aabb = getRectangleForEntity(zombieSprite, 1, 2);
	Color color = WHITE;

	if (isRedTimer > 0) { color = { 255,0,0,255 }; }

	DrawTexturePro(
		assetManager.zombie,
		getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft),
		aabb, // dest
		{ 0, 0 }, // origin top-left corner
		0.f, // rotation
		color // tint
	);

	//DrawTextureRec(
	//	assetManager.zombie, 
	//	(Rectangle) { 0, 0, facing, frameHeight }, 
	//	getPosition(), 
	//	WHITE
	//);

}

bool Zombie::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	// --- TIMERS ---
	if (isRedTimer > 0) isRedTimer -= deltaTime;
	changeStateTimer -= deltaTime;

	// --- DEATH CHECK (highest priority, overrides everything) ---
	if (life <= 0 && currentState != STATE_DEAD)
	{
		enterState(STATE_DEAD, entityUpdateData);
		return true;
	}

	if (currentState == STATE_DEAD) return false;

	// --- PERCEPTION ---
	Vector2 toPlayer = entityUpdateData.playerPosition - getPosition();
	float   distToPlayer = Vector2Length(toPlayer);
	bool    playerInSight = distToPlayer < SIGHT_RANGE;
	bool    playerInAttackRange = distToPlayer < ATTACK_RANGE;

	// --- STATE TRANSITIONS ---
	int previousState = currentState;

	//if (moveSpeed && shouldStepUp(entityUpdateData.playerPosition, entityUpdateData.gameMap))
	//{
	//	physics.jump(10);
	//}

	// set variable for flipping sprite
	if (moveSpeed >= 0)
	{
		animations.movingLeft = false;
	}
	else
	{
		animations.movingLeft = true;
	}

	switch (currentState)
	{
	case STATE_IDLE:
	{
		if (playerInSight)
			currentState = STATE_CHASING;
		else if (changeStateTimer <= 0)
		{
			changeStateTimer = WANDER_INTERVAL;
			currentState = STATE_WONDERING;
		}
		break;
	}


	case STATE_WONDERING:
	{
		if (playerInSight)
			currentState = STATE_CHASING;
		else if (changeStateTimer <= 0)
		{
			changeStateTimer = IDLE_INTERVAL;
			currentState = STATE_IDLE;
		}
		break;
	}

	case STATE_CHASING:
		if (!playerInSight)
			currentState = STATE_IDLE;         // lost the player
		else if (playerInAttackRange)
			currentState = STATE_ATTACK;
		break;

	case STATE_ATTACK:
		if (!playerInAttackRange)
			currentState = STATE_CHASING;
		else if (changeStateTimer <= 0)          // reuse timer as attack cooldown
		{
			changeStateTimer = ATTACK_COOLDOWN;
			doAttack(entityUpdateData);        // deal damage, play anim, etc.
		}
		break;

	case STATE_HURT:
		if (changeStateTimer <= 0)               // hurt stun duration expired
			currentState = (playerInSight) ? STATE_CHASING : STATE_IDLE;
		break;

	case STATE_DEAD:
		break;

	default:
		break;
	}

	bool justEnteredState = (previousState != currentState);
	if (justEnteredState)
		enterState(currentState, entityUpdateData);

	// --- MOVEMENT ---
	switch (currentState)
	{
	case STATE_IDLE:
	case STATE_ATTACK:
		moveSpeed = 0.f;
		break;

	case STATE_WONDERING:
		// direction already chosen in enterState
		break;

	case STATE_CHASING:
	{
		// walk toward player
		float dir = (toPlayer.x >= 0.f) ? 1.f : -1.f;
		moveSpeed = dir * CHASE_SPEED;
		break;
	}

	case STATE_HURT:
		moveSpeed = 0.f;
		break;

	default: break;
	}

	if (moveSpeed != 0.f)
	{
		getPosition().x += moveSpeed * deltaTime;

		if (shouldStepUp(entityUpdateData.playerPosition, entityUpdateData.gameMap))
			physics.jump(10.f);
	}

	// --- ANIMATION ---
	if (!physics.downTouch)
		animations.setAnimation(ANIM_JUMP);
	else if (moveSpeed == 0.f)
		animations.setAnimation(ANIM_IDLE);
	else
		animations.setAnimation(ANIM_WALK);

	animations.movingLeft = (moveSpeed < 0.f);
	animations.update(deltaTime, 0.08f, 7);

	return true;
}

// Called exactly once when transitioning INTO a new state
void Zombie::enterState(int newState, EntityUpdateData& entityUpdateData)
{
	switch (newState)
	{
	case STATE_IDLE:
		changeStateTimer = IDLE_INTERVAL;
		break;

	case STATE_WONDERING:
		moveSpeed = getRandomChance(entityUpdateData.rng, 0.5f) ? WANDER_SPEED : -WANDER_SPEED;
		changeStateTimer = WANDER_INTERVAL;
		break;

	case STATE_CHASING:
		break;

	case STATE_ATTACK:
		changeStateTimer = ATTACK_COOLDOWN;
		break;

	case STATE_HURT:
		changeStateTimer = HURT_DURATION;
		break;

	case STATE_DEAD:
		moveSpeed = 0.f;
		dropLoot(entityUpdateData.entityHolder, Block::boneChest);
		animations.setAnimation(ANIM_DEAD);
		break;

	default: break;
	}
}

void Zombie::doAttack(EntityUpdateData& entityUpdateData)
{
	entityUpdateData.playerPosition.x -= 1;
}


void Zombie::dropLoot(EntityHolder& entityHolder, int type)
{
	DroppedItem droppedItem;
	droppedItem.teleport(getPosition());

	// make it drop rarer chests with low chance
	droppedItem.itemType = type;
	droppedItem.physics.velocity.y = -3.f;

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.entities[id] = std::make_unique<DroppedItem>(droppedItem);
}


bool Zombie::shouldStepUp(Vector2 playerPos, GameMap& gameMap)
{
	Vector2 zTotPlayerDirection = playerPos - getPosition();

	int nextX = int(getPosition().x) + 1;
	int prevX = int(getPosition().x) - 1;

	auto bNext = gameMap.getBlockSafe(nextX, getPosition().y);
	auto bPrev = gameMap.getBlockSafe(prevX, getPosition().y);

	if (bNext && zTotPlayerDirection.x >= 0 && isCollidable(bNext->type))
	{
		return true;
	}

	if (bPrev && zTotPlayerDirection.x < 0 && isCollidable(bPrev->type))
	{
		return true;
	}
	return false;
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
