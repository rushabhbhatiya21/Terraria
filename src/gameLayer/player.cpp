#include "player.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>
#include <items/blocks.h>
#include "particles.h"

// todo: make AttackStyle struct and add enum to it, and have item manage update and spawn and all

void Player::render(AssetManager& assetManager)
{
	bool flashing = flashTimer > 0;

	if (flashing)
	{
		float flash = 1.f;

		BeginShaderMode(assetManager.flashShader);

		SetShaderValue(
			assetManager.flashShader,
			assetManager.flashShaderLocation,
			&flash,
			SHADER_UNIFORM_FLOAT
		);
	}

	drawSprite(assetManager);

	if (flashing)
	{
		EndShaderMode();
	}
}

void Player::drawSprite(AssetManager& assetManager)
{
	Transform2D playerSprite = physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;
	playerSprite.pos.y -= (playerSprite.h - physics.transform.h) / 2;

	auto aabb = playerSprite.getAABB();
	Rectangle textureUV = getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft);

	// --- BACK LAYER ---
	DrawTexturePro(assetManager.getFeetTexture(equipments.boots.itemId), textureUV, aabb, { 0,0 }, 0.f, WHITE);
	DrawTexturePro(assetManager.getHeadTexture(equipments.helmet.itemId), textureUV, aabb, { 0,0 }, 0.f, WHITE);
	DrawTexturePro(assetManager.getBackTexture(equipments.chest.itemId), textureUV, aabb, { 0,0 }, 0.f, WHITE);
	// --- FRONT LAYER ---
	DrawTexturePro(assetManager.getFrontTexture(equipments.chest.itemId), textureUV, aabb, { 0,0 }, 0.f, WHITE);

	ItemDefinition* item = getItem(heldItem);

	if (!item) return;

	switch (item->attackStyle)
	{
	case AttackStyle::SWING:
	{
		swingStyle.render(assetManager);
		break;
	}
	case AttackStyle::THRUST:
		break;
	case AttackStyle::THROW:
		break;
	case AttackStyle::SHOOT:
	{
		Texture2D textureItem   = getTextureForItemType(heldItem, assetManager);
		Rectangle textureUVItem = getTextureCoordinatesForItemType(heldItem, 32, 32, animations.movingLeft);

		Rectangle destRect{};
		destRect.width  = 1.0;
		destRect.height = 1.0;
		destRect.x = playerSprite.getCenter().x + (animations.movingLeft ? -.6f : .6f);
		destRect.y = playerSprite.getCenter().y;

		DrawTexturePro(textureItem, textureUVItem, destRect, { 0.5f,0.5f }, 0.f, WHITE);
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

	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	{
		inputX = -1.f;
		moving = true;
		animations.movingLeft = true;
	}
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	{
		inputX = 1.f;
		moving = true;
		animations.movingLeft = false;
	}

	physics.updateJump(
		deltaTime,
		IsKeyDown(KEY_SPACE),
		IsKeyPressed(KEY_SPACE)
	);

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