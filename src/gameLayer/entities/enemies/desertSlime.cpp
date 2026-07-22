#pragma once
#include "desetSlime.h"
#include <assets/assetManager.h>
#include <helper.h>
#include <randomStuff.h>
#include <entityHolder.h>
#include <player.h>
#include "entities/droppedItem.h"
#include <rendering/types/sprite.h>
#include <rendering/IRenderCollector.h>

//using Engine::AssetManager;

void DesertSlime::drawSprite(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	auto aabb = getRectangleForEntity(physics.transform, 1, 1);

	//DrawTexturePro(
	//	assetManager.desertSlime,
	//	getTextureAtlas(animations.positionX, animations.positionY, 32, 32, animations.movingLeft),
	//	aabb,
	//	{ 0,0 },
	//	0.f,
	//	WHITE
	//);

	Engine::Sprite sprite
	{
		getTextureAtlas(animations.positionX, animations.positionY, 32, 32, animations.movingLeft),
		aabb,
		{ 0,0 },
		0.f,
		Engine::White,
		assetManager.desertSlime,
		assetManager.defaultShader,
	};
	collector.submitSprite(sprite);
}

bool DesertSlime::update(float deltaTime, EntityUpdateData& data)
{
	changeStateTimer -= deltaTime;

	if (changeStateTimer < 0)
	{
		changeStateTimer = getRandomFloat(data.rng, 1, 7);

		float distance = Engine::Vec2Distance(getPosition(), data.player.getPosition());

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
			physics.jump(10);

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

void DesertSlime::dropLoot(int type, std::ranlux24_base& rng, EntityHolder& entityHolder)
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


Json DesertSlime::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// todo slime state

	return j;
}

bool DesertSlime::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}