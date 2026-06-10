#include "swingAttack.h"
#include "../assetManager.h"
#include "../entity.h"
#include "../helper.h"

void SwingAttack::render(AssetManager& assetManager)
{
	if (!owner) return;

	Texture2D  texture = getTextureForItemType(heldItem, assetManager);
	Rectangle  textureUVItem = getTextureCoordinatesForItemType(heldItem);

	auto     pos = owner->physics.transform.getAABB();
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

	DrawTexturePro(texture, textureUVItem, pos, origin, currentSwingAngle, WHITE);
}


void SwingAttack::update(float deltaTime)
{
	if (!owner) return;

	swingTimer = std::max(0.f, swingTimer - deltaTime);

	if (swingTimer <= 0.f)
	{
		isPlayingAttackAnimation = false;
		currentSwingAngle = 0.f;
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
	else { startAngle = 90.f; endAngle = -90.f; }

	currentSwingAngle = Lerp(startAngle, endAngle, t);

	float radians = DEG2RAD * currentSwingAngle;
	weaponTip.x = weaponBase.x + cosf(radians) * weaponLength;
	weaponTip.y = weaponBase.y + sinf(radians) * weaponLength;
}
