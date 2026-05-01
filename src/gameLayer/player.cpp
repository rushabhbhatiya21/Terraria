#include "player.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>
#include <inventory.h>

void Player::render(AssetManager& assetManager)
{
	Transform2D playerSprite = physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;

	playerSprite.pos.y -= (playerSprite.h - physics.transform.h) / 2;

	auto aabb = playerSprite.getAABB();

	auto textureUV = getTextureAtlas(animations.positionX, animations.positionY, 32, 64, animations.movingLeft);

	if (heldItem)
	{
		textureUV = getTextureAtlas(animations.positionX, animations.positionY + 3, 32, 64, animations.movingLeft);
	}

	DrawTexturePro(
		assetManager.getBackTexture(armourChest),
		textureUV,
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);

	DrawTexturePro(
		assetManager.getFeetTexture(armoutLegs),
		textureUV,
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);

	DrawTexturePro(
		assetManager.getHeadTexture(armourHead),
		textureUV,
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);

	if (heldItem)
	{
		Texture2D texture = getTextureForItemType(heldItem, assetManager);
		Rectangle textureUVItem = getTextureCoordinatesForItemType(heldItem);

		auto pos = aabb;

		if (heldItem < Block::BLOCKS_COUNT)
		{
			pos.width = 0.4;
			pos.height = 0.4;

			if (animations.movingLeft)
			{
				pos.y += 0.5;
				pos.x -= 0.2;
			}
			else
			{
				pos.y += 0.5;
				pos.x += 0.6;
			}
		}
		else
		{
			pos.width = 1;
			pos.height = 1;

			if (animations.movingLeft)
			{
				pos.y += 0.1;
				pos.x -= 0.7;
				textureUVItem = flipTextureAtlasX(textureUVItem);
			}
			else
			{
				pos.y += 0.1;
				pos.x += 0.5;
			}
		}

		float angle = 0.f;

		if (timeAfterAttackAnimation > 0)
		{
			float attackProgress = 1.0f - (timeAfterAttackAnimation / maxAttackTimeAnimation);

			float t = attackProgress;
			t = t * t * (3 - 2 * t); // smoothstep

			angle = -120.0f + t * 180.0f;
		}

		Vector2 origin = { 0,0 };

		DrawTexturePro(
			texture,
			textureUVItem,
			pos,
			origin,
			angle,
			WHITE
		);
	}

	DrawTexturePro(
		assetManager.getFrontTexture(armourChest),
		textureUV,
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);

	DrawRectangleLinesEx(
		aabb,
		0.1f,
		{ 20,101,250,120 }
	);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	return true;
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