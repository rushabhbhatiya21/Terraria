#pragma once
#include "slime.h"
#include "../droppedItem.h"
#include <helper.h>
#include <assets/assetManager.h>
#include <entityHolder.h>
#include <items/blocks.h>
#include <player.h>
#include "combat/combatSystem.h"
#include "shake.h"
#include "ui/popupText.h"

//using Engine::AssetManager;

void Slime::drawSprite(Engine::AssetManager& assetManager)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);
	Color color = WHITE;

	if (flashTimer > 0) { color = { 255,0,0,255 }; }

	//DrawTexturePro(
	//	assetManager.slime,
	//	getTextureAtlas(animations.positionX, animations.positionY, 32, 32, animations.movingLeft),
	//	aabb, // dest
	//	{ 0, 0 }, // origin top-left corner
	//	0.f, // rotation
	//	color // tint
	//);
}

bool Slime::update(float deltaTime, EntityUpdateData& data)
{
	if (flashTimer > 0)
	{
		flashTimer -= deltaTime;
	}

	changeStateTimer -= deltaTime;

	if (life <= 0)
	{
		currentState = STATE_DEAD;
	}

	// damage player
	// add attack state
	//if (physics.transform.intersectTransform(data.player.physics.transform))
	//{
	//	doAttack(&data.player);
	//}

	isColliding = physics.transform.intersectTransform(data.player.physics.transform);

	if (isColliding && !wasColliding)
	{
		wasColliding = true;
		doAttack(&data.player);
	}
	wasColliding = isColliding;

	if (changeStateTimer < 0)
	{
		changeStateTimer = getRandomFloat(data.rng, 1, 7);

		float distance = Vector2Distance(getPosition(), data.player.getPosition());

		if (distance < 20.f)
		{
			if (getRandomChance(data.rng, 0.8f))
			{
				currentState = STATE_CHASING;
			}
			else
			{
				currentState = STATE_WONDERING;
			}
		}
		else
		{
			currentState = STATE_WONDERING;
		}
	}

	if (physics.downTouch)
	{
		moveSpeed = 0.f;
		animations.setAnimation(0); // on ground
	}
	else
	{
		animations.setAnimation(1); // in air
	}

	jumpTimer -= deltaTime;

	switch (currentState)
	{
		case STATE_WONDERING:
			if (jumpTimer < 0)
			{
				jumpTimer = getRandomFloat(data.rng, 3, 12);
				physics.jump(10);
				moveSpeed = getRandomFloat(data.rng, -7, 7);
			}
			break;

		case STATE_CHASING:
			if (jumpTimer < 0)
			{
				jumpTimer = getRandomFloat(data.rng, 2, 7);
				physics.jump(7);

				if (data.player.getPosition().x > getPosition().x)
				{
					moveSpeed = getRandomFloat(data.rng, 1, 7);
				}
				else
				{
					moveSpeed = -getRandomFloat(data.rng, 1, 7);
				}
			}
			break;

		case STATE_DEAD:
		{
			// drop loop
			dropLoot(Items::slime, data.rng, data.entityHolder);
			return false;
		}

		default:
			break;
	}

	if (moveSpeed)
	{
		getPosition().x += deltaTime * moveSpeed;
	}

	animations.update(deltaTime, 0.08f, 7);

	return true;

}

// put this in enemy.h
void Slime::doAttack(Player* player)
{
	// implement attack here
	DamageInfo info;
	info.attacker = this;
	info.hitDirection = getPosition() - player->getPosition();

	CombatSystem::applyDamage(player, info);


	return;
}

void Slime::dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder)
{
	int dropsCount = 0;
	//bool legendary = false;
	//bool epic      = false;
	//bool rare      = false;
	//bool common    = false;

	if (getRandomChance(rng, .05f))
		dropsCount = 10;
	else if (getRandomChance(rng, .15f))
		dropsCount = 6;
	else if (getRandomChance(rng, .35f))
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

Json Slime::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo slime state

	return j;
}

bool Slime::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}
