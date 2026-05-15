#include "player.h"
#include <helper.h>
#include <assetManager.h>
#include <entityHolder.h>
#include <items/blocks.h>
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
		float rotation = 0;
		float angle = 0;
		Vector2 origin = { 0,1 };

		if (heldItem < Block::BLOCKS_COUNT)
		{
			pos.width = 0.4f;
			pos.height = 0.4f;

			if (animations.movingLeft)
			{
				pos.y += 0.5f;
				pos.x -= 0.2f;
			}
			else
			{
				pos.y += 0.5f;
				pos.x += 0.6f;
			}
		}
		else
		{
			pos.width = 1.f;
			pos.height = 1.f;

			if (animations.movingLeft)
			{
				pos.y += 1.2f;
				pos.x += 0.2f;
				angle = 120.f;
				origin = { 1.f,1.f };
				textureUVItem = flipTextureAtlasX(textureUVItem);
			}
			else
			{
				pos.y += 1.2f;
				pos.x += 0.8f;
				angle = -120.f;
			}
		}

		if (isPlayingAttackAnimation)
		{
			float attackProgress = 1.f - (useTimer / attackDuration);

			attackProgress = Clamp(attackProgress, 0.f, 1.f);

			float t = attackProgress;

			// smoothstep
			t = t * t * (3.f - 2.f * t);

			if (animations.movingLeft)
			{
				t *= -1.f;
			}

			rotation = angle + t * 180.f;
		}

		DrawTexturePro(
			texture,
			textureUVItem,
			pos,
			origin,
			rotation,
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

	//DrawRectangleLinesEx(
	//	aabb,
	//	0.1f,
	//	{ 20,101,250,120 }
	//);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	useTimer -= deltaTime;

	if (useTimer < 0.f)
	{
		useTimer = 0.f;
		isPlayingAttackAnimation = false;
	}

	if (useTimer > 0)
	{
		weaponBase = getPosition();

		float t = 1.f - (useTimer / attackDuration);
		//float t = 0;

		float startAngle = 0, endAngle = 0;

		if (!animations.movingLeft)
		{
			startAngle = -135.f;   // 10 o'clock (after negation in radians)
			endAngle = 135.f;    // 4 o'clock
		}
		else
		{
			startAngle = -45.f;    // mirrored
			endAngle = 45.f;
		}

		//float radians = DEG2RAD * (-currentAngle); // keep negation

		float currentAngle = Lerp(startAngle, endAngle, t);
		//float currentAngle = -45.f;
		float radians = DEG2RAD * (-currentAngle);

		weaponTip.x = weaponBase.x + cosf(radians) * weaponLength;
		weaponTip.y = weaponBase.y + sinf(radians) * weaponLength;

		isPlayingAttackAnimation = true;
	}

	return true;
}

void Player::dropLoot(EntityHolder& entityHolder, int type)
{
	// no need to implement
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