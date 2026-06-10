#include "swingAttack.h"
#include "../assetManager.h"
#include "../entity.h"

void SwingAttack::render(AssetManager& assetManager)
{
	if (!owner) return;

	Texture2D  texture = getTextureForItemType(heldItem, assetManager);
	Rectangle  textureUVItem = getTextureCoordinatesForItemType(heldItem);

	auto     pos = aabb;
	float    rotation = 0.f;
	Vector2  origin = { 0, 1 };

	pos.width = 1.f;
	pos.height = 1.f;

	if (owner->animations.movingLeft)
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

	// rotation is now just a read — all math lives in updateSwing()
	if (isPlayingAttackAnimation)
	{
		float t = 1.f - (swingTimer / attackDuration);
		t = Clamp(t, 0.f, 1.f);
		t = t * t * (3.f - 2.f * t);

		if (!owner->animations.movingLeft) rotation = Lerp(-90.f, 90.f, t);
		else                               rotation = Lerp(90.f, -90.f, t);
	}

	DrawTexturePro(texture, textureUVItem, pos, origin, rotation, WHITE);
}


void SwingAttack::update(float deltaTime)
{
	// update timers
	if (swingTimer > 0.f)
	{
		swingTimer -= deltaTime;
	}
	else
	{
		swingTimer = 0.f;
		isPlayingAttackAnimation = false;
	}

	if (swingTimer <= 0.f)
	{
		isPlayingAttackAnimation = false;
		return;
	}

	// swing logic
	isPlayingAttackAnimation = true;
	weaponBase = owner->getPosition();

	float t = 1.f - (swingTimer / attackDuration);
	t = Clamp(t, 0.f, 1.f);
	t = t * t * (3.f - 2.f * t);

	float startAngle, endAngle;
	if (!owner->animations.movingLeft) { startAngle = -90.f; endAngle = 90.f; }
	else { startAngle = 270.f; endAngle = 90.f; }

	float currentAngle = Lerp(startAngle, endAngle, t);
	float radians = DEG2RAD * currentAngle;
	weaponTip.x = weaponBase.x + cosf(radians) * weaponLength;
	weaponTip.y = weaponBase.y + sinf(radians) * weaponLength;
}
