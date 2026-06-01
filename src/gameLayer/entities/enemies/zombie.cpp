#pragma once
#include "zombie.h"
#include "../droppedItem.h"
#include <items/blocks.h>
#include <gameMap.h>
#include <assetManager.h>
#include <helper.h>
#include <entityHolder.h>
#include <player.h>
#include "../../combat/combatSystem.h"
#include "../../ui/popupText.h"
#include "../../shake.h"

void Zombie::drawSprite(AssetManager& assetManager)
{
	Transform2D zombieSprite = physics.transform;
	zombieSprite.w = 1;
	zombieSprite.h = 2;

	zombieSprite.pos.y -= (zombieSprite.h - physics.transform.h) / 2;

	auto aabb = getRectangleForEntity(zombieSprite, 1, 2);

	DrawTexturePro(
		assetManager.zombie,
		getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft),
		aabb, // dest
		{ 0, 0 }, // origin top-left corner
		0.f, // rotation
		WHITE // tint
	);

	if (flashTimer > 0)
	{
		float flash = (flashTimer > 0) ? 1.0f : 0.0f;

		BeginShaderMode(assetManager.flashShader);

		SetShaderValue(
			assetManager.flashShader,
			GetShaderLocation(assetManager.flashShader, "flash"),
			&flash,
			SHADER_UNIFORM_FLOAT
		);

		DrawTexturePro(
			assetManager.zombie,
			getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft),
			aabb,
			{ 0,0 },
			0.f,
			Color{ 255,255,255,255 }
		);
		EndShaderMode();
	}
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

	Vector2 dirVector = data.player.getPosition() - getPosition();
	float dist = Vector2Length(dirVector);
	//printf("windup: %f, recover: %f\n", attackWindUpTimer, attackRecoverTimer);
	//if (previouseState != currentState)
	//printf("state: %d, hurtTimer: %f\n", currentState, hurtTimer);
	//printf("timer: %f, state: %d, movespeed: %f, dist: %f\n", changeStateTimer, currentState, moveSpeed, dist);

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
		moveSpeed = WANDER_SPEED * (movingLeft ? -1.f : 1.f);
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
		break;
	}
	case Zombie_State::ATTACK_RECOVER:
	{
		moveSpeed = 0;
		break;
	}
	case Zombie_State::HURT:
	{
		moveSpeed = 0;
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
		movingLeft = !movingLeft;
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
	info.item = nullptr;
	info.hitDirection = getPosition() - player->getPosition();

	DamageResult& result = CombatSystem::applyDamage(player, info);

	float shakeDuration = result.crit ? .2f : .1f;
	float shakeStength = result.crit ? .3f : .2f;
	camShake.triggerCameraShake(shakeDuration, shakeStength);

	spawnPopupText(
		player->getPosition(),
		Vector2{ .1f, .1f },
		std::to_string(int(std::floor(result.finalDamage))),
		1,
		.4f,
		-1.f,
		WHITE,
		result.crit
	);

	return;
}

void Zombie::onHit()
{
	Enemy::onHit();
	enterState(Zombie_State::HURT);
}

void Zombie::dropLoot(EntityHolder& entityHolder, int type)
{
	DroppedItem droppedItem;
	droppedItem.teleport(getPosition());

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

	auto bNext = gameMap.getBlockSafe(nextX, (int)getPosition().y);
	auto bPrev = gameMap.getBlockSafe(prevX, (int)getPosition().y);

	if (bNext && zTotPlayerDirection.x >= 0 && isCollidable(bNext->type))
		return true;

	if (bPrev && zTotPlayerDirection.x < 0 && isCollidable(bPrev->type))
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
		return !bNext || !isCollidable(bNext->type);
	}
	else if (moveSpeed < 0.f) // moving left, check left side
	{
		int prevX = int(getPosition().x) - 1;
		auto bPrev = gameMap.getBlockSafe(prevX, belowY);
		return !bPrev || !isCollidable(bPrev->type);
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


//bool Zombie::update(float deltaTime, EntityUpdateData& data)
//{
//	// --- HITSTOP ---
//	if (hitStopTimer > 0)
//	{
//		hitStopTimer -= deltaTime;
//		physics.velocity = { 0.f, 0.f };
//		return true; // skip ALL logic and physics this frame
//	}
//
//	// --- TIMERS ---
//	changeStateTimer -= deltaTime;
//
//	// --- DEATH CHECK (highest priority, overrides everything) ---
//	if (!isAlive && currentState != STATE_DEAD)
//	{
//		enterState(STATE_DEAD, data);
//		return false;
//	}
//
//	if (currentState == STATE_DEAD) return false;
//
//	// --- PERCEPTION ---
//	Vector2 toPlayer = data.player.getPosition() - getPosition();
//	float   distToPlayer = Vector2Length(toPlayer);
//	bool    playerInSight = distToPlayer < SIGHT_RANGE;
//	bool    playerInAttackRange = distToPlayer < ATTACK_RANGE;
//	bool    isOnEdge = isOnLedge(data.gameMap);
//
//	// --- STATE TRANSITIONS ---
//	int previousState = currentState;
//
//	// set variable for flipping sprite
//	if (moveSpeed >= 0)
//		animations.movingLeft = false;
//	else
//		animations.movingLeft = true;
//
//	switch (currentState)
//	{
//	case STATE_IDLE:
//	{
//		if (playerInSight && !isOnEdge)
//			currentState = STATE_CHASING;
//		else if (isOnEdge || changeStateTimer <= 0) // force wander immediately if on edge
//		{
//			changeStateTimer = WANDER_INTERVAL;
//			currentState = STATE_WONDERING;
//		}
//		break;
//	}
//
//	case STATE_WONDERING:
//	{
//		if (isOnEdge) // hit a ledge while wandering, reverse direction
//			moveSpeed = -moveSpeed;
//		else if (changeStateTimer <= 0)
//		{
//			changeStateTimer = IDLE_INTERVAL;
//			currentState = STATE_IDLE;
//		}
//		break;
//	}
//
//	case STATE_CHASING:
//		if (!playerInSight)
//			currentState = STATE_IDLE;
//		else if (isOnEdge)
//			currentState = STATE_WONDERING;
//		else if (playerInAttackRange)
//			currentState = STATE_ATTACK;
//		break;
//
//	case STATE_ATTACK:
//		if (isOnEdge)
//			currentState = STATE_IDLE;
//		else if (!playerInAttackRange)
//			currentState = STATE_CHASING;
//		else if (changeStateTimer <= 0) // reuse timer as attack cooldown
//		{
//			changeStateTimer = ATTACK_COOLDOWN;
//			doAttack(data);
//		}
//		break;
//
//	case STATE_HURT:
//		if (changeStateTimer <= 0) // hurt stun duration expired
//			currentState = (playerInSight) ? STATE_CHASING : STATE_IDLE;
//		break;
//
//	case STATE_DEAD:
//		break;
//
//	default:
//		break;
//	}
//
//	bool justEnteredState = (previousState != currentState);
//	if (justEnteredState)
//		enterState(currentState, data);
//
//	// --- MOVEMENT ---
//	switch (currentState)
//	{
//	case STATE_IDLE:
//	case STATE_ATTACK:
//		moveSpeed = 0.f;
//		break;
//
//	case STATE_WONDERING:
//		// direction already chosen in enterState
//		break;
//
//	case STATE_CHASING:
//	{
//		float dir = (toPlayer.x >= 0.f) ? 1.f : -1.f;
//		moveSpeed = dir * CHASE_SPEED;
//		break;
//	}
//
//	case STATE_HURT:
//		moveSpeed = 0.f;
//		break;
//
//	default: break;
//	}
//
//	if (moveSpeed != 0.f)
//	{
//		getPosition().x += moveSpeed * deltaTime;
//
//		if (shouldStepUp(data.player.getPosition(), data.gameMap))
//			physics.jump(10.f);
//	}
//
//	// --- ANIMATION ---
//	if (!physics.downTouch)
//		animations.setAnimation(ANIM_JUMP);
//	else if (moveSpeed == 0.f)
//		animations.setAnimation(ANIM_IDLE);
//	else
//		animations.setAnimation(ANIM_WALK);
//
//	animations.movingLeft = (moveSpeed < 0.f);
//	animations.update(deltaTime, 0.08f, 7);
//
//	return true;
//}
//
//// Called exactly once when transitioning INTO a new state
//void Zombie::enterState(int newState, EntityUpdateData& data)
//{
//	switch (newState)
//	{
//	case STATE_IDLE:
//		changeStateTimer = IDLE_INTERVAL;
//		break;
//
//	case STATE_WONDERING:
//	{
//		Vector2 toPlayer = data.player.getPosition() - getPosition();
//		bool onEdge = isOnLedge(data.gameMap);
//
//		if (onEdge)
//		{
//			// walk away from player and give enough time to clear the ledge
//			moveSpeed = (toPlayer.x >= 0.f) ? -WANDER_SPEED : WANDER_SPEED;
//			changeStateTimer = WANDER_INTERVAL * 3.f;
//		}
//		else
//		{
//			moveSpeed = getRandomChance(data.rng, 0.5f) ? WANDER_SPEED : -WANDER_SPEED;
//			changeStateTimer = WANDER_INTERVAL;
//		}
//		break;
//	}
//
//	case STATE_CHASING:
//		break;
//
//	case STATE_ATTACK:
//		changeStateTimer = ATTACK_COOLDOWN;
//		break;
//
//	case STATE_HURT:
//		changeStateTimer = HURT_DURATION;
//		break;
//
//	case STATE_DEAD:
//		moveSpeed = 0.f;
//		dropLoot(data.entityHolder, Block::boneChest);
//		animations.setAnimation(ANIM_DEAD);
//		break;
//
//	default: break;
//	}
//}
