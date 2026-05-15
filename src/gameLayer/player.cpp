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
				pos.y += 1.f;
				pos.x += 0.2f;
				textureUVItem = flipTextureAtlasX(textureUVItem);
				origin = { 1.f, 1.f };
			}
			else
			{
				pos.y += 1.f;
				pos.x += .8f;
				origin = { 0.f, 1.f };
			}
		}

		if (isPlayingAttackAnimation)
		{
			float t = 1.f - (swingTimer / attackDuration);
			t = Clamp(t, 0.f, 1.f);
			t = t * t * (3.f - 2.f * t); // smooth step

			float startAngle, endAngle;
			// right - 0
			if (!animations.movingLeft)
			{
				startAngle = -90.f;
				endAngle = 90.f;
			}
			// left - 0
			else
			{
				startAngle = 90;
				endAngle = -90;
			}

			rotation = Lerp(startAngle, endAngle, t);
		}

		DrawTexturePro(
			texture,
			textureUVItem,
			pos,
			origin,
			rotation,
			WHITE
		);

		// debug heldItem box
		//DrawRectangleLinesEx(pos, .1f, RED);

	}

	DrawTexturePro(
		assetManager.getFrontTexture(armourChest),
		textureUV,
		aabb,
		{ 0,0 },
		0.f,
		WHITE
	);

	// debug player box
	//DrawRectangleLinesEx(
	//	aabb,
	//	0.1f,
	//	{ 20,101,250,120 }
	//);
}

bool Player::update(float deltaTime, EntityUpdateData entityUpdateData)
{
	// cooldown timer
	useTimer -= deltaTime;
	if (useTimer <= 0.f)
	{
		useTimer = 0.f;
	}

	// swing timer
	swingTimer -= deltaTime;
	if (swingTimer <= 0.f)
	{
		swingTimer = 0.f;
		isPlayingAttackAnimation = false;
	}

	if (swingTimer > 0.f)
	{
		weaponBase = getPosition();
		float t = 1.f - (swingTimer / attackDuration);
		t = Clamp(t, 0.f, 1.f);

		float startAngle = 0, endAngle = 0;
		// right - 0
		if (!animations.movingLeft)
		{
			startAngle = -90.f;
			endAngle = 90.f;
		}
		// left - 180
		else
		{
			startAngle = 270.f;
			endAngle = 90.f;
		}

		float currentAngle = Lerp(startAngle, endAngle, t);
		float radians = DEG2RAD * currentAngle;
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