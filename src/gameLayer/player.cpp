#include "player.h"
#include <input/input.h>
#include <helper.h>
#include <assets/assetManager.h>
#include <entityHolder.h>
#include <items/blocks.h>
#include "particles.h"
#include <rendering/types/sprite.h>
#include <rendering/IRenderCollector.h>

// todo: make AttackStyle struct and add enum to it, and have item manage update and spawn and all

void Player::render(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	bool flashing = flashTimer > 0;
	//
	if (flashing)
	{
		float flash = 1.f;
	//
	//	BeginShaderMode(assetManager.flashShader);
	//
	//	SetShaderValue(
	//		assetManager.flashShader,
	//		assetManager.flashShaderLocation,
	//		&flash,
	//		SHADER_UNIFORM_FLOAT
	//	);
	}

	drawSprite(assetManager, collector);

	//if (flashing)
	//{
	//	EndShaderMode();
	//}
}

void Player::drawSprite(Engine::AssetManager& assetManager, Engine::IRenderCollector& collector)
{
	Transform2D playerSprite = physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;
	playerSprite.pos.y -= (playerSprite.h - physics.transform.h) / 2;

	auto aabb = playerSprite.getAABB();
	Engine::Rect textureUV = getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft);

	// --- BACK LAYER ---
	//DrawTexturePro(assetManager.getFeetTexture(equipments.boots.itemId), textureUV, aabb, { 0,0 }, 0.f, Engine::White);
	//DrawTexturePro(assetManager.getHeadTexture(equipments.helmet.itemId), textureUV, aabb, { 0,0 }, 0.f, Engine::White);
	//DrawTexturePro(assetManager.getBackTexture(equipments.chest.itemId), textureUV, aabb, { 0,0 }, 0.f, Engine::White);
	// --- FRONT LAYER ---
	//DrawTexturePro(assetManager.getFrontTexture(equipments.chest.itemId), textureUV, aabb, { 0,0 }, 0.f, Engine::White);

	bool flashing = flashTimer > 0;
	if (flashing)
	{
		float flash = 1.f;
	}

	Engine::Sprite bootsSprite
	{
		&assetManager.getFeetTexture(equipments.boots.itemId),
		&assetManager.flashShader,
		//nullptr,
		textureUV,
		aabb,
		{0,0},
		0.f,
		Engine::White,
		flashing
	};

	Engine::Sprite helmetSprite
	{
		&assetManager.getHeadTexture(equipments.helmet.itemId),
		&assetManager.flashShader,
		//nullptr,
		textureUV,
		aabb,
		{0,0},
		0.f,
		Engine::White,
		flashing
	};

	Engine::Sprite backChestSprite
	{
		&assetManager.getBackTexture(equipments.chest.itemId),
		&assetManager.flashShader,
		//nullptr,
		textureUV,
		aabb,
		{0,0},
		0.f,
		Engine::White,
		flashing
	};

	Engine::Sprite frontChectSprite
	{
		&assetManager.getFrontTexture(equipments.chest.itemId),
		&assetManager.flashShader,
		//nullptr,
		textureUV,
		aabb,
		{0,0},
		0.f,
		Engine::White,
		flashing
	};

	collector.submitSprite(bootsSprite);
	collector.submitSprite(helmetSprite);
	collector.submitSprite(backChestSprite);
	collector.submitSprite(frontChectSprite);

	ItemDefinition* item = getItem(heldItem);

	if (!item) return;

	switch (item->attackStyle)
	{
	case AttackStyle::SWING:
	{
		swingStyle.render(assetManager, collector);
		break;
	}
	case AttackStyle::THRUST:
		break;
	case AttackStyle::THROW:
		break;
	case AttackStyle::SHOOT:
	{
		auto& textureItem = getTextureForItemType(heldItem, assetManager);
		Engine::Rect textureUVItem = getTextureCoordinatesForItemType(heldItem, 32, 32, animations.movingLeft);

		Engine::Rect destRect{};
		destRect.width = 1.0;
		destRect.height = 1.0;
		destRect.x = playerSprite.getCenter().x + (animations.movingLeft ? -.6f : .6f);
		destRect.y = playerSprite.getCenter().y;

		//DrawTexturePro(textureItem, textureUVItem, destRect, { 0.5f,0.5f }, 0.f, Engine::White);
		Engine::Sprite heldItemSprite
		{
			&textureItem,
			&assetManager.defaultShader,
			textureUVItem,
			destRect,
			{.5f,.5f},
			.0f,
			Engine::White
		};
		collector.submitSprite(heldItemSprite);
		break;
	}
	case AttackStyle::CAST:
		break;
	default:
		break;
	}
}

bool Player::update(float deltaTime, EntityUpdateData& data)
{
	// --- HITSTOP ---
	if (hitStopTimer > 0)
	{
		hitStopTimer -= deltaTime;
		physics.velocity = { 0.f, 0.f };
		return true; // stop all movements
	}

	if (life <= 0) return false;

	updateTimers(deltaTime);
	updateMovement(deltaTime);
	updateSwing(deltaTime, data);
	updateAnimation(deltaTime);

	return true;
}

void Player::updateTimers(float deltaTime)
{
	if (useTimer > 0.f)
		useTimer -= deltaTime;

	if (flashTimer > 0)
		flashTimer -= deltaTime;
}

void Player::updateMovement(float deltaTime)
{
	float inputX = 0.f;
	bool moving = false;
	bool droppedThrough = false;

	if (Engine::isKeyDown(Engine::Key::Left) || Engine::isKeyDown(Engine::Key::A))
	{
		inputX = -1.f;
		moving = true;
		animations.movingLeft = true;
	}
	if (Engine::isKeyDown(Engine::Key::Right) || Engine::isKeyDown(Engine::Key::D))
	{
		inputX = 1.f;
		moving = true;
		animations.movingLeft = false;
	}
	if ((Engine::isKeyDown(Engine::Key::Down) || Engine::isKeyDown(Engine::Key::S)) && Engine::isKeyPressed(Engine::Key::Space) && physics.standingOnPlatform)
	{
		printf("down jump pressed\n");
		physics.dropThroughTimer = 0.2f;
		droppedThrough = true;
	}

	if (!droppedThrough)
	{
		physics.updateJump(
			deltaTime,
			Engine::isKeyDown(Engine::Key::Space),
			Engine::isKeyPressed(Engine::Key::Space)
		);
	}

	physics.applyGravity();
	physics.applyHorizontalMovement(deltaTime, inputX);
}

void Player::updateSwing(float deltaTime, EntityUpdateData& data)
{
	ItemDefinition* item = getItem(heldItem);
	if (!item) return;

	std::vector<Particle> dummyParticles;

	switch (item->attackStyle)
	{
	case AttackStyle::SWING:
	{
		swingStyle.updateAnimation(deltaTime);
		swingStyle.updateSwings(deltaTime, data.gameMap, data.entityHolder, dummyParticles, data.rng); // replace particles here
		break;
	}
	case AttackStyle::THRUST:
		break;
	case AttackStyle::THROW:
		break;
	case AttackStyle::SHOOT:
		break;
	case AttackStyle::CAST:
		break;
	default:
		break;
	}
}

void Player::updateAnimation(float deltaTime)
{
	bool falling = !physics.downTouch;

	if (falling && physics.velocity.y < 0.f)
		animations.setAnimation(2); // jump (rising)
	else if (falling)
		animations.setAnimation(0); // fall (descending)
	else if (physics.velocity.x != 0.f)
		animations.setAnimation(1); // walk
	else
		animations.setAnimation(0); // idle

	if (swingStyle.isPlayingAnimation)
		animations.setAnimation(3); // assuming swing animation

	animations.update(deltaTime, 0.08f, 7);
}

//void Player::dropLoot(EntityHolder& entityHolder, int type)
//{
//	// no need to implement
//}

void Player::onHit()
{
	flashTimer = .15f;
}

Json Player::formatToJson()
{
	Json j;
	addCommonEntityStuffToJson(j);

	// add more data to save here

	return j;
}

bool Player::loadFromJson(Json& j)
{
	*this = {};

	bool rez = loadCommonEntityStuffFromJson(j);

	setColliderSize();

	return rez;
}