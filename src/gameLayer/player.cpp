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
		textureUV = getTextureAtlas(animations.positionX, animations.positionY + 3, 32, 64, animations.movingLeft);

	// --- BACK LAYER ---
	DrawTexturePro(assetManager.getBackTexture(equipments.chest.itemId),  textureUV, aabb, { 0,0 }, 0.f, WHITE);
	DrawTexturePro(assetManager.getFeetTexture(equipments.boots.itemId),  textureUV, aabb, { 0,0 }, 0.f, WHITE);
	DrawTexturePro(assetManager.getHeadTexture(equipments.helmet.itemId), textureUV, aabb, { 0,0 }, 0.f, WHITE);

	// --- HELD ITEM ---
	if (heldItem)
	{
		Texture2D  texture = getTextureForItemType(heldItem, assetManager);
		Rectangle  textureUVItem = getTextureCoordinatesForItemType(heldItem);

		auto     pos = aabb;
		float    rotation = 0.f;
		Vector2  origin = { 0, 1 };

		if (heldItem < Block::BLOCKS_COUNT)
		{
			pos.width = 0.4f;
			pos.height = 0.4f;

			if (animations.movingLeft) { pos.y += 0.5f; pos.x -= 0.2f; }
			else { pos.y += 0.5f; pos.x += 0.6f; }
		}
		else
		{
			//renderTrail();

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
				pos.x += 0.8f;
				origin = { 0.f, 1.f };
			}
		}

		// rotation is now just a read — all math lives in updateSwing()
		if (isPlayingAttackAnimation)
		{
			float t = 1.f - (swingTimer / attackDuration);
			t = Clamp(t, 0.f, 1.f);
			t = t * t * (3.f - 2.f * t);

			if (!animations.movingLeft) rotation = Lerp(-90.f, 90.f, t);
			else                        rotation = Lerp(90.f, -90.f, t);
		}

		DrawTexturePro(texture, textureUVItem, pos, origin, rotation, WHITE);
	}

	// --- FRONT LAYER ---
	DrawTexturePro(assetManager.getFrontTexture(equipments.chest.itemId), textureUV, aabb, { 0,0 }, 0.f, WHITE);
}

//void Player::renderTrailPass(float expand, unsigned char maxAlpha)
//{
//	if (trailCount < 1) return;
//
//	for (int i = trailCount - 1; i > 0; i--)
//	{
//		int idxA = (trailHead - i + TRAIL_SIZE) % TRAIL_SIZE;
//		int idxB = (trailHead - i - 1 + TRAIL_SIZE) % TRAIL_SIZE;
//
//		auto& a = trailPoints[idxA];
//		auto& b = trailPoints[idxB];
//
//		auto expandPoint = [](Vector2 base, Vector2 tip, float amount) -> Vector2 {
//			Vector2 dir = Vector2Normalize(Vector2Subtract(tip, base));
//			return { tip.x + dir.x * amount, tip.y + dir.y * amount };
//			};
//
//		Vector2 tipA = expandPoint(a.base, a.tip, expand);
//		Vector2 tipB = expandPoint(b.base, b.tip, expand);
//
//		unsigned char alphaA = (unsigned char)(a.alpha * maxAlpha);
//		unsigned char alphaB = (unsigned char)(b.alpha * maxAlpha);
//
//		Color colA = { 255, 220, 100, alphaA };
//		Color colB = { 255, 220, 100, alphaB };
//
//		if (!animations.movingLeft)
//		{
//			DrawTriangle(a.base, tipA, tipB, colA);
//			DrawTriangle(a.base, tipB, b.base, colB);
//		}
//		else // swap winding order for left-facing
//		{
//			DrawTriangle(a.base, tipB, tipA, colA);
//			DrawTriangle(a.base, b.base, tipB, colB);
//		}
//	}
//}
//
//void Player::renderTrail()
//{
//	if (!isPlayingAttackAnimation || trailCount < 2) return;
//
//	BeginBlendMode(BLEND_ADDITIVE); // KEY: additive blending makes it actually glow
//
//	renderTrailPass(0.3f, 30);  // outer glow  — wide,  faint
//	renderTrailPass(0.15f, 80);  // inner glow  — medium
//	renderTrailPass(0.f, 180);  // core        — sharp, bright
//
//	EndBlendMode();
//}

bool Player::update(float deltaTime, EntityUpdateData& data)
{
	// --- HITSTOP ---
	if (hitStopTimer > 0)
	{
		hitStopTimer -= deltaTime;
		physics.velocity = { 0.f, 0.f };
		return true;
	}

	if (life <= 0) return false;

	updateTimers(deltaTime);
	updateMovement(deltaTime);
	updateSwing(deltaTime);
	updateAnimation(deltaTime);

	return true;
}

void Player::updateTimers(float deltaTime)
{
	if (useTimer > 0.f)
		useTimer -= deltaTime;

	if (swingTimer > 0.f)
		swingTimer -= deltaTime;

	if (swingTimer <= 0.f)
	{
		swingTimer = 0.f;
		isPlayingAttackAnimation = false;
	}
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

void Player::updateSwing(float deltaTime)
{
	//if (resetTrailNextFrame)
	//{
	//	trailCount = 0;
	//	trailHead = 0;
	//	weaponTip = { 0.f, 0.f }; // add this
	//	resetTrailNextFrame = false;
	//}

	if (swingTimer <= 0.f)
	{
		isPlayingAttackAnimation = false;
		return;
	}

	//printf("swingTimer: %.3f  trailCount: %d  weaponTip: %.2f %.2f\n",
	//	swingTimer, trailCount, weaponTip.x, weaponTip.y);

	isPlayingAttackAnimation = true;
	weaponBase = getPosition();

	float t = 1.f - (swingTimer / attackDuration);
	t = Clamp(t, 0.f, 1.f);
	t = t * t * (3.f - 2.f * t);

	float startAngle, endAngle;
	if (!animations.movingLeft) { startAngle = -90.f; endAngle = 90.f; }
	else { startAngle = 270.f; endAngle = 90.f; }

	float currentAngle = Lerp(startAngle, endAngle, t);
	float radians = DEG2RAD * currentAngle;
	weaponTip.x = weaponBase.x + cosf(radians) * weaponLength;
	weaponTip.y = weaponBase.y + sinf(radians) * weaponLength;

	//// replace t > 0.01f guard with this
	//if (weaponTip.x == 0.f && weaponTip.y == 0.f)
	//	return;

	//trailHead = (trailHead + 1) % TRAIL_SIZE;
	//trailPoints[trailHead] = { weaponTip, weaponBase, 1.f };
	//if (trailCount < TRAIL_SIZE) trailCount++;

	//for (int i = 0; i < trailCount; i++)
	//{
	//	int idx = (trailHead - i + TRAIL_SIZE) % TRAIL_SIZE;
	//	trailPoints[idx].alpha = 1.f - (float(i) / float(trailCount));
	//}
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

	animations.update(deltaTime, 0.08f, 7);
}

//void Player::dropLoot(EntityHolder& entityHolder, int type)
//{
//	// no need to implement
//}

void Player::onHit()
{
	//flashTimer = .2f;
	//hurtTimer = 1;
	//currentState = STATE_HURT;
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