#include "gameplay.h"
#include <chrono>
#include <iostream>
#include <imgui.h>

#include <ui.h>
#include <audio.h>
#include <settings.h>
#include <helper.h>
#include <saveMap.h>
#include <worldGenerator.h>
#include <crafting.h>

#include <shake.h>
#include <lighting.h>
#include <rendering/worldRenderer.h>

#include <entities/droppedItem.h>
#include <entities/enemies/enemy.h>
#include <entities/enemies/slime.h>
#include <entities/enemies/desetSlime.h>
#include <entities/enemies/zombie.h>
#include <entities/enemies/evilEye.h>
#include <entities/enemies/evilEyeServant.h>

#include <items/item.h>
#include <items/itemUse.h>

#include <combat/blockSpawn.h>

#include "ui/popupText.h"

#pragma region sky colors

// Bright, slightly warm white — more natural than pure white
const Color dayColor = { 255, 250, 230, 255 };

// Deeper, richer sky blue — more contrast against terrain
const Color daySky = { 85, 170, 255, 255 };

// Sunrise — cool pink/gold, feels like early morning mist
const Color sunriseA = { 255, 200, 160, 255 };  // warm gold horizon
const Color sunriseB = { 200, 160, 220, 255 };  // soft lavender upper sky
const Color sunriseSky = { 255, 180, 140, 255 };  // peachy horizon glow

// Sunset — deeper, moodier, clearly different from sunrise
const Color sunsetA = { 255, 100,  60, 255 };  // burnt orange-red
const Color sunsetB = { 120,  50, 140, 255 };  // deep violet
const Color sunsetSky = { 100,  60, 130, 255 };  // dusky purple

// Night — slightly cooler/deeper, more contrast at night
const Color nightColor = { 20,  35,  80, 255 };
const Color nightSky = { 8,  12,  40, 255 };  // near-black deep blue

#pragma endregion


#pragma region hotbar keys

const KeyboardKey hotbarKeys[] =
{
	KEY_ONE,
	KEY_TWO,
	KEY_THREE,
	KEY_FOUR,
	KEY_FIVE,
	KEY_SIX,
	KEY_SEVEN,
	KEY_EIGHT,
	KEY_NINE,
	KEY_ZERO
};

#pragma endregion


#pragma region sky color helpers

static Color lerpColor(Color a, Color b, float t)
{
	return
	{
		(unsigned char)(a.r + (b.r - a.r) * t),
		(unsigned char)(a.g + (b.g - a.g) * t),
		(unsigned char)(a.b + (b.b - a.b) * t),
		255
	};
}

static SkyData getSkyData(float t)
{
	// Night → Night
	if (t < 0.25f)
	{
		return { nightSky, nightColor, 0.65f, DayPhase::NIGHT };
	}
	// Sunrise  06:00–09:00
	else if (t < 0.375f)
	{
		float p = (t - 0.25f) / 0.125f;
		return {
			lerpColor(nightSky,   daySky,    p),
			lerpColor(nightColor, dayColor,   p),
			0.65f * (1.0f - p),
			DayPhase::SUNRISE
		};
	}
	// Day  09:00–18:00
	else if (t < 0.75f)
	{
		return { daySky, dayColor, 0.0f, DayPhase::DAY };
	}
	// Sunset  18:00–21:00
	else if (t < 0.875f)
	{
		float p = (t - 0.75f) / 0.125f;
		return {
			lerpColor(daySky,    nightSky,   p),
			lerpColor(dayColor,  nightColor, p),
			0.65f * p,
			DayPhase::SUNSET
		};
	}
	// Night  21:00–24:00
	return { nightSky, nightColor, 0.65f, DayPhase::NIGHT };
}

#pragma endregion


#pragma region day/night cyle helpers

bool Gameplay::isNight(float t)
{
	return (t >= 0.875f || t < 0.25f);
}

float Gameplay::getDayPercent(float t)
{
	// todo implement
	return 0;
}

WorldTimeClock Gameplay::getWorldTimeClock(float t)
{
	int totalMinutes = (int)(t * 1440);

	int h = totalMinutes / 60;
	int m = totalMinutes % 60;

	return WorldTimeClock{ h, m, 0 };
}

#pragma endregion


#pragma region spawn helpers

void Gameplay::spawnDroppedItem(Vector2 position, int type)
{
	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	auto item = std::make_unique<DroppedItem>();
	item->teleport(position);
	item->itemType = type;
	item->physics.velocity.y = -3.f;

	DroppedItem* itemPtr = item.get();
	entityHolder.entities[id] = std::move(item);
	entityHolder.droppedItems.push_back(itemPtr);
}

#pragma endregion


#pragma region rect ui helpers

Rectangle Gameplay::getInventoryRectangle(float w, float h)
{
	Rectangle inventoryRectangle = {};

	inventoryRectangle.height = h * .3f;
	inventoryRectangle.width = inventoryRectangle.height * 3.34; // 3 for 9 cols, 3.34 for 10 cols

	float maxWidth = w * .9f;
	if (inventoryRectangle.width > maxWidth)
	{
		float scaleFactor = maxWidth / inventoryRectangle.width;
		inventoryRectangle.height *= scaleFactor;
		inventoryRectangle.width *= scaleFactor;
	}

	inventoryRectangle = placeRectangleTopLeft(inventoryRectangle);

	inventoryRectangle.x += w * .01f;
	inventoryRectangle.y += h * .03f;

	return inventoryRectangle;
}

Rectangle Gameplay::getCraftRectangle(float w, float h)
{
	Rectangle craftRectangle = {};

	// Base size
	craftRectangle.width = w * 0.25f;
	craftRectangle.height = h * 0.45f;

	//// Clamp
	//craftRectangle.width = Clamp(craftRectangle.width, 180.f, 420.f);
	//craftRectangle.height = Clamp(craftRectangle.height, 260.f, 420.f);

	// Bottom left
	craftRectangle = placeRectangleBottomLeftCorner(craftRectangle, h);

	// Fixed padding
	float padding = 12.f;

	craftRectangle.x += padding;
	craftRectangle.y -= padding;

	return craftRectangle;
}

Rectangle Gameplay::getRecipeRectangle(float w, float h, Rectangle craftRectangle)
{
	Rectangle recipeRectangle = {};

	float padding = 10.f;

	// 1/3 width
	recipeRectangle.width =	craftRectangle.width * 0.2f;

	// Fixed internal padding
	recipeRectangle.height = craftRectangle.height - padding * 2.f;

	// Inside parent
	recipeRectangle.x =	craftRectangle.x + padding;

	recipeRectangle.y =	craftRectangle.y + padding;

	return recipeRectangle;
}

Rectangle Gameplay::getIngredientsRectangle(float w, float h, Rectangle craftRectangle, Rectangle recipeRectangle)
{
	Rectangle ingredientRectangle = {};

	float padding = 10.f;

	// Remaining width
	ingredientRectangle.width = craftRectangle.width - recipeRectangle.width - padding * 3.f;

	// Almost full height
	ingredientRectangle.height = craftRectangle.height - padding * 2.f;

	// Position to the RIGHT of recipe panel
	ingredientRectangle.x =	recipeRectangle.x +	recipeRectangle.width +	padding;

	ingredientRectangle.y =	craftRectangle.y + padding;

	return ingredientRectangle;
}

void Gameplay::drawInventoryBackground(const Rectangle& inventoryRectangle, const Inventory& inventory, bool insideInventory)
{
	// hotbar always visible
	DrawRectangle(
		(int)inventoryRectangle.x,
		(int)inventoryRectangle.y,
		(int)inventoryRectangle.width,
		(int)inventoryRectangle.height / inventory.rows,
		{ 100,100,100,100 }
	);

	// remaining rows only when inventory is open
	if (insideInventory)
	{
		DrawRectangle(
			(int)inventoryRectangle.x,
			(int)inventoryRectangle.y + (int)inventoryRectangle.height / inventory.rows,
			(int)inventoryRectangle.width,
			(int)inventoryRectangle.height * (inventory.rows - 1) / inventory.rows,
			{ 100,100,100,100 }
		);
	}
}

void Gameplay::drawInventorySlot(bool isDragged, const Rectangle& rect, const ItemStack& stack, bool selected, AssetManager& assetManager)
{
	Color c = { 180,180,200,240 };

	if (CheckCollisionPointRec(GetMousePosition(), rect))
		c = { 220,250,220,250 };

	if (selected)
		c = { 255,230,80,255 };

	DrawTexturePro(
		assetManager.frame,
		getTextureAtlas(0, 0,
			assetManager.frame.width,
			assetManager.frame.height),
		rect,
		{ 0,0 },
		0.f,
		c
	);

	auto atlas = getTextureCoordinatesForItemType(stack.itemId);
	Texture2D tex = getTextureForItemType(stack.itemId, assetManager);

	if (!isDragged)
	{
		DrawTexturePro(
			tex,
			atlas,
			shrinkRectanglePercentage(rect, .3f, .3f),
			{ 0,0 },
			0.f,
			c
		);

		if (stack.count != 0 && isStackable(stack.itemId))
		{
			Vector2 textPos =
			{
				rect.x + rect.width * 0.5f,
				rect.y + rect.height * 0.75f
			};
			std::string str = std::to_string(stack.count);
			Vector2 textSize = MeasureTextEx(GetFontDefault(), str.c_str(), 25.f, 2.f);

			// write item count as text
			DrawTextPro(
				GetFontDefault(),
				str.c_str(),
				textPos,
				{ textSize.x / 2.f, textSize.y / 2.f },
				0.f,
				25.f,
				1.f,
				{ 255, 255, 255, 200 }
			);
		}
	}

}

Rectangle Gameplay::getInventorySlotRect(int index, const Rectangle& inventoryRectangle, const Inventory& inventory)
{
	Rectangle rect = shrinkRectanglePercentage(
		inventoryRectangle,
		0.01f,
		0.01f
	);

	float cellHeight = rect.height / inventory.rows;
	float cellWidth = cellHeight;

	int row = index / inventory.columns;
	int col = index % inventory.columns;

	Rectangle newRect;
	newRect.x = rect.x + col * cellWidth;
	newRect.y = rect.y + row * cellHeight;
	newRect.width = cellWidth;
	newRect.height = cellHeight;

	newRect = shrinkRectanglePercentage(
		newRect,
		0.1f,
		0.1f
	);

	return newRect;
}

void Gameplay::drawInventorySlotByIndex(int index, bool isDragged, const Rectangle& inventoryRectangle, const Inventory& inventory, const Player& player, AssetManager& assetManager)
{
	Rectangle slotRect = getInventorySlotRect(index, inventoryRectangle, inventory);

	const ItemStack& stack = inventory.slots[index];

	drawInventorySlot(
		isDragged,
		slotRect,
		stack,
		player.selectedHotbarSlot == index,
		assetManager
	);
}

int Gameplay::getHoveredInventorySlot(Vector2 mousePos, Rectangle inventoryRectangle, const Inventory& inventory, bool insideInventory)
{
	inventoryRectangle = shrinkRectanglePercentage(
		inventoryRectangle,
		0.01f,
		0.01f
	);

	float cellHeight = inventoryRectangle.height / inventory.rows;
	float cellWidth = cellHeight;

	int maxRows = insideInventory ? inventory.rows : 1;

	float rawCol = (mousePos.x - inventoryRectangle.x) / cellWidth;
	float rawRow = (mousePos.y - inventoryRectangle.y) / cellHeight;

	if (rawCol < 0 || rawRow < 0 )
		return -1;

	int col = (int)rawCol;
	int row = (int)rawRow;

	if (col >= inventory.columns || row >= maxRows)
		return -1;

	return row * inventory.columns + col;
}

void Gameplay::drawDraggedItem(const ItemStack& stack, AssetManager& assetManager)
{
	Vector2 mouse = GetMousePosition();

	Rectangle r;
	r.width = 48;
	r.height = 48;
	r.x = mouse.x - r.width / 2.f;
	r.y = mouse.y - r.height / 2.f;

	auto atlas = getTextureCoordinatesForItemType(stack.itemId);

	Texture2D tex = getTextureForItemType(stack.itemId, assetManager);

	DrawTexturePro(
		tex,
		atlas,
		r,
		{ 0,0 },
		0.f,
		Color{ 255,255,255,180 }
	);
}

void Gameplay::drawDisplauNameUI(ItemId itemId, Rectangle parentRect, float fontSize, float spacing, float paddingX, float paddingY, Color rectColor, Color textColor)
{
	ItemDefinition* selectedIngredient = getItem(itemId);

	if (!selectedIngredient)
		return;

	//float fontSize = 9.f;
	//float spacing = 1.f;
	//float paddingX = 6.f;
	//float paddingY = 2.f;

	Vector2 textSize = MeasureTextEx(GetFontDefault(), selectedIngredient->displayName, fontSize, spacing);

	Rectangle rectPos{
		parentRect.x + parentRect.width / 2 - (textSize.x + paddingX * 2) / 2, // center horizontally
		parentRect.y - textSize.y - paddingY * 2 - 5,                   // above item slot
		textSize.x + paddingX * 2,
		textSize.y + paddingY * 2
	};

	DrawRectangleRounded(
		rectPos,
		.7f,
		1,
		//Color{ 255,255,255,200 }
		rectColor
	);

	DrawTextPro(
		GetFontDefault(),
		selectedIngredient->displayName,
		{
			rectPos.x + paddingX,
			rectPos.y + paddingY
		},
		{ 0, 0 },
		0.f,
		fontSize,
		spacing,
		textColor
	);
}

#pragma endregion


#pragma region craft helpers

Recipes::CraftingStation Gameplay::getNearbyStation(Vector2 playerPos)
{
	Recipes::CraftingStation nearbyCraftingStation = Recipes::CraftingStation::NONE;
	for (int y = playerPos.y - 3; y <= playerPos.y + 3; y++)
	{
		for (int x = playerPos.x - 3; x <= playerPos.x + 3; x++)
		{
			auto b = gameMap.getBlockSafe(x, y);

			if (!b) break;

			if (b->type == Items::workBench)
			{
				nearbyCraftingStation = Recipes::CraftingStation::WORKBENCH;
				break;
			}

			if (b->type == Items::furnace)
			{
				nearbyCraftingStation = Recipes::CraftingStation::FURNACE;
				break;
			}
		}

		if (nearbyCraftingStation != Recipes::CraftingStation::NONE) break;
	}

	return nearbyCraftingStation;
}

#pragma endregion


#pragma region light helpers

//void Gameplay::addLight(int worldX, int worldY, float radius, float intensity, bool isTorch)
//{
//	// visible lightmap range
//	int lightW = (int)lightMap.size();
//	int lightH = (int)lightMap[0].size();
//
//	for (int dy = -(int)radius; dy <= (int)radius; dy++)
//	{
//		for (int dx = -(int)radius; dx <= (int)radius; dx++)
//		{
//			float distSq = (float)(dx * dx + dy * dy);
//			float radiusSq = radius * radius;
//
//			if (distSq > radiusSq)
//				continue;
//
//			// convert world -> local lightmap coords
//			int lx = worldX + dx;
//			int ly = worldY + dy;
//
//			if (lx < 0 || ly < 0 || lx >= lightW || ly >= lightH)
//				continue;
//
//			// smooth falloff
//			float t = 1.0f - (sqrtf(distSq) / (float)radius);
//
//			// gamma curve (VERY IMPORTANT)
//			t = powf(t, 2.f);
//
//			// convert to light value
//			float lightValue = t * intensity;
//
//			// optional wall attenuation
//			if (!isTorch)
//			{
//				auto* b = gameMap.getBlockSafe(
//					lx,
//					ly - 2
//				);
//
//				auto* bDef = getItem(b->type);
//
//				if (b && bDef && bDef->block.isCollidable()
//				)
//				{
//					lightValue *= 0.3f;
//				}
//			}
//
//			lightValue = Clamp(lightValue, 0.0f, 1.0f);
//
//			// keep brightest light
//			lightMap[lx][ly] = std::max(lightMap[lx][ly], lightValue);
//		}
//	}
//}

//void Gameplay::floodFillLight(int x, int y, int offsetX, int offsetY, int value, bool isTorch)
//{
//	if (x < 0 || y < 0 || x > lightMap.size() - 1 || y > lightMap[0].size() - 1) return;
//
//	auto* b = gameMap.getBlockSafe(x + offsetX, y + offsetY - 2);
//
//	if (!b) return;
//
//	if (isTorch)
//		value -= 1;
//
//	if (!isTorch && b->isCollidable())
//		value -= 1;
//
//	if (lightMap[x][y] >= value) return;
//	
//	lightMap[x][y] = value;
//
//	floodFillLight(x - 1, y, offsetX, offsetY, value, isTorch); // left
//	floodFillLight(x, y - 1, offsetX, offsetY, value, isTorch); // top
//	floodFillLight(x + 1, y, offsetX, offsetY, value, isTorch); // right
//	floodFillLight(x, y + 1, offsetX, offsetY, value, isTorch); // bottom
//}

static float tileNoise(int x, int y)
{
	unsigned int n = x * 374761393u + y * 668265263u;
	n = (n ^ (n >> 13u)) * 1274126177u;
	n ^= (n >> 16u);

	return (n & 255) / 255.f;
}

#pragma endregion


bool Gameplay::init(AssetManager& assetManager)
{
	double loadStart = GetTime();

	// items init
	registerItems();

	// map init
	const int w = 912, h = 512;
	backgroundMap.create(w, h);
	generateWorld(gameMap, w, h);

	// renderer
	renderer.init(gameMap, assetManager, spriteBatch);

	// lighting init
	recalculateLight(gameMap);

	// chunk texture init - no longer using it (too much memory consumption)
	//ChunkRendererTexture::initializeChunkRenderTextures(gameMap);

	// cam init
	camera.target = { 20, 120 };
	camera.rotation = 0.f;
	camera.zoom = CAMERA_ZOOM;

	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	// player spawn
	player.teleport({ 20, 60 });

	// Use a RenderTexture to capture the scene
	sceneTexture =        LoadRenderTexture(screenW, screenH);
	lightMask =           LoadRenderTexture(screenW / LIGHT_SCALE, screenH / LIGHT_SCALE);
	glowTexture =         LoadRenderTexture(screenW / LIGHT_SCALE, screenH / LIGHT_SCALE);
	blurredLightTexture = LoadRenderTexture(screenW / LIGHT_SCALE, screenH / LIGHT_SCALE);
	blurredGlowTexture =  LoadRenderTexture(screenW / LIGHT_SCALE, screenH / LIGHT_SCALE);

	SetTextureFilter(lightMask.texture,           TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(glowTexture.texture,         TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(blurredLightTexture.texture, TEXTURE_FILTER_BILINEAR);
	SetTextureFilter(blurredGlowTexture.texture,  TEXTURE_FILTER_BILINEAR);

	// cam foloow player
	camFollow.init(1.5f, .74f, 1.f, player.getPosition());

	//spawnEnemyHelper<EvilEye>({ 35,55 });
	//spawnEnemyHelper<EvilEyeServant>({ 35,55 });
	//spawnEnemyHelper<Slime>({ 31,60 });
	//spawnEnemyHelper<Slime>({ 29,60 });
	//spawnEnemyHelper<Slime>({ 32,60 });
	//spawnEnemyHelper<Zombie>({ 25,60 });
	//spawnDroppedItem({ 25, 60 }, Items::goldHelmet);
	maxEnemyCount = 1;

	// start item in inventory
	player.inventory.storeItem(ItemStack{ Items::woodenSword, 1 });
	player.inventory.storeItem(ItemStack{ Items::woodAxe, 1 });
	//player.inventory.storeItem(ItemStack{ Items::woodLog, 20 });
	player.inventory.storeItem(ItemStack{ Items::shuriken, 100 });
	player.inventory.storeItem(ItemStack{ Items::woodenBow, 1 });
	player.inventory.storeItem(ItemStack{ Items::woodenArrow, 100 });
	player.inventory.storeItem(ItemStack{ Items::platform, 100 });
	//player.inventory.storeItem(ItemStack{ Items::woodLog, 100 });
	//player.inventory.storeItem(ItemStack{ Items::leaves, 100 });
	player.inventory.storeItem(ItemStack{ Items::furnace, 10 });
	//player.inventory.storeItem(ItemStack{ Items::workBench, 1 });
	//player.inventory.storeItem(ItemStack{ Items::copperIngot, 20 });

	// start day at random time
	std::ranlux24_base rng(std::random_device{}());
	float randStartTime = getRandomFloat(rng, 0.4f, 0.6f);
	worldTime = randStartTime * FULL_DAY_LENGTH;

	// chunk grid testing
	std::cout << sizeof(Block) << "\n";
	std::cout << sizeof(Chunk) << "\n";
	std::cout << sizeof(ChunkGrid) << "\n";
	std::cout << sizeof(Gameplay) << "\n";

	double loadEnd = GetTime();
	TraceLog(LOG_INFO, "Load time: %.3f seconds", loadEnd - loadStart);
	TraceLog(LOG_INFO, "Raylib version: %s", RAYLIB_VERSION);

	return true;
}

bool Gameplay::update(AssetManager& assetManager)
{

#pragma region delta time

	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5.f) deltaTime = 1 / 5.f;

#pragma endregion


#pragma region world time

	worldTime += deltaTime;
	if (worldTime >= FULL_DAY_LENGTH)
		worldTime -= FULL_DAY_LENGTH;

	float t = worldTime / FULL_DAY_LENGTH;

	SkyData skyData = getSkyData(t);
	Color ambientColor = skyData.ambientColor;
	float darkness = skyData.darkness;

#pragma endregion


#pragma region updates (deltatime dependent)

	updateShake(deltaTime);
	camShake.updateCameraShake(deltaTime);
	updateParticles(deltaTime);
	updatePopupText(deltaTime);

#pragma endregion


#pragma region camera offset and smoothing

	camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };

	Vector2 camOffset = { 0, 0 };

	if (camShake.time > 0.0f)
	{
		float t = camShake.time / camShake.duration;
		float strength = camShake.strength * t;

		float time = (float)GetTime();

		camOffset.x = sinf(time * 30.0f + camShake.phase) * strength;
		camOffset.y = cosf(time * 30.0f + camShake.phase) * strength;
	}

	Vector2 smoothTarget = camFollow.Update(deltaTime, player.getPosition());

	camera.target.x = smoothTarget.x + camOffset.x;
	camera.target.y = smoothTarget.y + camOffset.y;

#pragma endregion


#pragma region key bindings

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

	if (IsKeyPressed(KEY_TAB)) { insideInventory = !insideInventory; }

	if (IsKeyPressed(KEY_C)) { insideCraft = !insideCraft; }

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) { saveWorld(gameMap, entityHolder, player); }

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) { loadWorld(gameMap, entityHolder, player); }

#pragma endregion


#pragma region creative mode

	std::ranlux24_base rng(std::random_device{}());

	static bool creative = false;

	// snapshot HERE, before any physics or input runs
	bool wasTouchingGround = player.physics.downTouch;
	float landingVelocity = player.physics.velocity.y;

	// ── Creative mode fly ────────────────────────────────────────────────────
	if (creative)
	{
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
			player.physics.transform.pos.y -= PhysicalEntity::MOVE_SPEED * deltaTime;
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
			player.physics.transform.pos.y += PhysicalEntity::MOVE_SPEED * deltaTime;
		if (GetMouseWheelMove() != 0.f)
			camera.zoom += GetMouseWheelMove();
	}


#pragma endregion


#pragma region handle player

	auto updateEntityPhysics = [&](auto& entity, bool applyGravity = true, bool shouldResolveConstraints = true)
		{
			if (applyGravity)             { entity.physics.applyGravity(); }
			entity.physics.updateForces(deltaTime);
			if (shouldResolveConstraints) { entity.physics.resolveConstrains(gameMap); }
			entity.physics.updateFinal();
			entity.physics.updatePhysicsTimers(deltaTime);
		};

	updateEntityPhysics(player, player.shouldApplyGravity); // gravity = false for player

	bool justLanded = !wasTouchingGround && player.physics.downTouch;

	// spawn particles on player land
	if (justLanded)
	{
		int blockType = 1;
		// spawn particles at feet
		Vector2 playerPos = player.physics.transform.getBottom();
		Vector2 playerPosLeft = player.physics.transform.getBottomLeft();

		// get block below feet
		// check collider left
		auto b = gameMap.getBlockSafe((int)playerPosLeft.x, (int)playerPosLeft.y);
		if (b && b->type != Items::air)
		{
			blockType = b->type;
		}
		else
		{
			// check collider right
			Vector2 playerPosRight = player.physics.transform.getBottomRight();
			auto b = gameMap.getBlockSafe((int)playerPosRight.x, (int)playerPosRight.y);
			if (b && b->type != Items::air) blockType = b->type;
		}

		// spawn your particles here
		//auto rightParticles = 
		spawnParticles(
			playerPos,
			rng,
			blockType,
			(player.numberOfParticlesOnLand * int(landingVelocity)) / 2,
			player.physics.transform.w,
			45
		);

		spawnParticles(
			playerPos,
			rng,
			blockType,
			(player.numberOfParticlesOnLand * int(landingVelocity)) / 2,
			player.physics.transform.w,
			-45
		);
	}

	player.isAlive = player.update(deltaTime, EntityUpdateData
		{
			player,
			rng,
			entityHolder,
			player.inventory,
			gameMap
		}
	);

	if (!player.isAlive)
	{
		player = Player{};
		player.teleport({ 20,60 });
	}

#pragma endregion


#pragma region clamp camera

	// clamp camera
	{
		float zoom = camera.zoom;

		float screenWidth  = (float)GetScreenWidth();
		float screenHeight = (float)GetScreenHeight();

		// half of visible area (adjusted for zoom)
		float halfViewWidth = (screenWidth * 0.5f) / zoom;
		float halfViewHeight = (screenHeight * 0.5f) / zoom;

		float minX = halfViewWidth;
		float maxX = gameMap.w - halfViewWidth;
		float minY = halfViewHeight;
		float maxY = gameMap.h - halfViewHeight;

		// if map is smaller than view (zoomed out a lot), we set the camera to center of the world
		if (maxX < minX)
		{
			camera.target.x = gameMap.w * 0.5f;
		}
		else
		{
			camera.target.x = Clamp(camera.target.x, minX, maxX);
		}

		if (maxY < minY)
		{
			camera.target.y = gameMap.h * 0.5f;
		}
		else
		{
			camera.target.y = Clamp(camera.target.y, minY, maxY);
		}
	}

#pragma endregion


#pragma region handle entities

	// update all entities
	for (auto it = entityHolder.entities.begin(); it != entityHolder.entities.end(); ++it)
	{
		EntityUpdateData entityUpdateData
		{
			player,
			rng,
			entityHolder,
			player.inventory,
			gameMap,
			it->first
		};

		// if update false, kill entity
		if (!it->second->update(deltaTime, entityUpdateData))
		{
			it->second->isAlive = false;
		}
		// update physics
		else
		{
			updateEntityPhysics(*it->second, it->second->shouldApplyGravity, it->second->shouldResolveConstraints);
		}
	}

	// cleanup after update
	entityHolder.cleanup();

#pragma endregion


#pragma region handle inventory

	for (int i = 0; i < player.inventory.slots.size(); i++)
	{
		if (player.inventory.slots[i].itemId != 0 && player.inventory.slots[i].count <= 0)
		{
			player.inventory.removeItem(i);

			// clear selection
			creativeSelectedBlock = 0;
			player.heldItem = 0;
			player.recalculateStats();
		}
	}

#pragma endregion


#pragma region handle input

	Rectangle inventoryRectangle = getInventoryRectangle((float)GetScreenWidth(), (float)GetScreenHeight());

	// inside hotbar meu
	bool insideHotbarMenu = false;
	Rectangle hotbarRectangle = inventoryRectangle;
	hotbarRectangle.height /= player.inventory.rows;
	insideHotbarMenu = CheckCollisionPointRec(GetMousePosition(), hotbarRectangle);

	// inside inventory except hotbar
	bool insideInventoryGrid = false;
	insideInventoryGrid = CheckCollisionPointRec(GetMousePosition(), inventoryRectangle) && insideInventory;

	// inside craft menu
	bool insideCraftingMenu = false;
	Rectangle craftRectangle = getCraftRectangle((float)GetScreenWidth(), (float)GetScreenHeight());
	insideCraftingMenu = CheckCollisionPointRec(GetMousePosition(), craftRectangle) && insideCraft;

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (creativeSelectedBlock < 0) { creativeSelectedBlock = 0; }
	if (creativeSelectedBlock >= Items::LAST_BLOCK) { creativeSelectedBlock = Items::LAST_BLOCK - 1; }

	// selection
	if (showImgui)
	{
		if (IsKeyPressed(KEY_ONE))
		{
			selectionStart = Vector2{ float(blockX), float(blockY) };
		}
		if (IsKeyPressed(KEY_TWO))
		{
			selectionEnd = Vector2{ float(blockX), float(blockY) };
		}
		if (IsKeyPressed(KEY_THREE))
		{
			copyStructure.pasteIntoMap(gameMap, Vector2{ float(blockX), float(blockY) });
		}

		if (selectionStart.x > selectionEnd.x)
		{
			std::swap(selectionStart.x, selectionEnd.x);
		}
		if (selectionStart.y > selectionEnd.y)
		{
			std::swap(selectionStart.y, selectionEnd.y);
		}

	}

	if (!showImgui)
	{
		if (!insideInventoryGrid && !insideCraftingMenu && !insideHotbarMenu)
		{
			if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_MIDDLE))
			{
				auto b = gameMap.getBlockSafe(blockX, blockY);

				if (b)
				{
					creativeSelectedBlock = b->type;
				}
			}
		}

		if (!insideInventoryGrid && !insideCraftingMenu && !insideHotbarMenu)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				// attack, tool, place block, projectile done
				// todo consumable 

				useItem(
					&player,
					player.inventory.slots[player.selectedHotbarSlot],
					entityHolder,
					gameMap,
					worldPos
				);
			}
		}
	}

#pragma endregion


#pragma region handle blocks

	updateBlock(gameMap, player.inventory);

#pragma endregion


#pragma region get screen coords

	// visible tile range dynamically based on screen resolution and zoom
	float visibleWorldWidth = GetScreenWidth() / camera.zoom;
	float visibleWorldHeight = GetScreenHeight() / camera.zoom;

	int visibleTilesX = (int)ceilf(visibleWorldWidth / TILE_SIZE);
	int visibleTilesY = (int)ceilf(visibleWorldHeight / TILE_SIZE);

	// padding
	visibleTilesX += 4;
	visibleTilesY += 4;

	int startXView = (int)(camera.target.x / TILE_SIZE) - visibleTilesX / 2;
	int endXView = startXView + visibleTilesX;

	int startYView = (int)(camera.target.y / TILE_SIZE) - visibleTilesY / 2;
	int endYView = startYView + visibleTilesY;

	// clamp
	startXView = std::max(0, startXView);
	startYView = std::max(0, startYView);

	endXView = std::min(gameMap.w - 1, endXView);
	endYView = std::min(gameMap.h - 1, endYView);

#pragma endregion


#pragma region enemy spawner

	enemySpawner.enemySpawnTimer -= deltaTime;

	int enemyCount = entityHolder.enemies.size();

	// spawn more enemies at night
	if (isNight(t))
	{
		maxEnemyCount = getRandomInt(rng, 10, 15);
	}
	else
	{
		maxEnemyCount = getRandomInt(rng, 5, 8);
	}

	//maxEnemyCount = 5;

	//if (enemySpawner.enemySpawnTimer <= 0 && enemyCount < maxEnemyCount)
	//{
	//	spawnEnemy(entityHolder, gameMap, rng, startXView, endXView, startYView, endYView);
	//	enemySpawner.enemySpawnTimer = 2;
	//}

#pragma endregion


#pragma region draw background

	// background
	{
		int backgroundType = DrawBackground::forest;

		if (player.getPosition().x > gameMap.desertStart &&
			player.getPosition().x < gameMap.desertEnd)
		{
			backgroundType = DrawBackground::desert;
		}

		if (player.getPosition().y > 130)
		{
			backgroundType = DrawBackground::cave;
		}

		if (isNight(t))
		{
			backgroundType = DrawBackground::night;
		}

		background.setBackground(backgroundType);

		background.draw(
			deltaTime,
			assetManager,
			camera,
			{ (float)gameMap.w, (float)gameMap.h },
			skyData.skyColor
		);
	}

#pragma endregion


#pragma region adjust lightmask for screen size

	// resize render textures if screen resized
	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	if (lightMask.id == 0 ||
		sceneTexture.id == 0 ||
		glowTexture.id == 0 ||
		blurredLightTexture.id == 0 ||
		blurredGlowTexture.id == 0 ||
		screenW != lastScreenWidth ||
		screenH != lastScreenHeight)
	{
		if (lightMask.id != 0)
			UnloadRenderTexture(lightMask);

		if (sceneTexture.id != 0)
			UnloadRenderTexture(sceneTexture);

		if (glowTexture.id != 0)
			UnloadRenderTexture(glowTexture);

		if (blurredLightTexture.id != 0)
			UnloadRenderTexture(blurredLightTexture);

		if (blurredGlowTexture.id != 0)
			UnloadRenderTexture(blurredGlowTexture);

		lightMask = LoadRenderTexture(screenW, screenH);
		sceneTexture = LoadRenderTexture(screenW, screenH);
		glowTexture = LoadRenderTexture(screenW, screenH);
		blurredLightTexture = LoadRenderTexture(screenW, screenH);
		blurredGlowTexture = LoadRenderTexture(screenW, screenH);

		lastScreenWidth = screenW;
		lastScreenHeight = screenH;
	}

#pragma endregion


	// below 4 commented out for now
#pragma region build tile lightmap

	//// reset light map
	//lightMap.assign(
	//	endXView - startXView + 1,
	//	std::vector<float>(endYView - startYView + 1, 0.0f)
	//);

#pragma endregion


#pragma region add light sources

	// player light
	//{
	//	int localX = (int)player.getPosition().x - startXView;
	//	int localY = (int)player.getPosition().y - startYView;

	//	addLight(
	//		localX,
	//		localY,
	//		10.f,
	//		1.f
	//	);
	//}


	// torch light example
	//{
	//	int torchX = 29;
	//	int torchY = 59;

	//	float intensity = 0.92f + sinf(::GetTime() * 23.0f) * 0.10f + sinf(::GetTime() * 37.0f) * 0.05f;

	//	addLight(
	//		torchX - startXView,
	//		torchY - startYView,
	//		8.f,
	//		intensity,
	//		true
	//	);
	//}

#pragma endregion


#pragma region build lightmask texture

	//BeginTextureMode(lightMask);
	//
	//unsigned char ambientByte = (unsigned char)(0.08f * 255.f);

	//ClearBackground(Color{
	//	ambientByte,
	//	ambientByte,
	//	ambientByte,
	//	255
	//});

	//BeginMode2D(camera);

	//for (int x = startXView; x <= endXView; x++)
	//{
	//	for (int y = startYView; y <= endYView; y++)
	//	{
	//		float light = lightMap[x - startXView][y - startYView];

	//		float ambient = 0.08f;

	//		float t = ambient + light * (1.0f - ambient);

	//		t = Clamp(t, 0.0f, 1.0f);

	//		unsigned char c = (unsigned char)(t * 255.0f);

	//		DrawRectangle(
	//			x,
	//			y,
	//			1,
	//			1,
	//			Color{ c, c, c, 255 }
	//		);
	//	}
	//}

	//EndMode2D();
	//EndTextureMode();

#pragma endregion


#pragma region add blur pass

	//BeginTextureMode(blurredLightTexture);

	//BeginShaderMode(assetManager.blurShader);

	//ClearBackground(BLACK);

	//DrawTextureRec(
	//	lightMask.texture,
	//	{
	//		0,
	//		0,
	//		(float)lightMask.texture.width,
	//		-(float)lightMask.texture.height
	//	},
	//	{ 0,0 },
	//	WHITE
	//);

	//EndShaderMode();

	//EndTextureMode();

#pragma endregion
	

	BeginTextureMode(sceneTexture);
	ClearBackground(BLANK);
	BeginMode2D(camera);


#pragma region render world

	//int debugRendering = 2;

	renderer.drawBlocks(startYView, endYView, startXView, endXView);

	//auto start = std::chrono::high_resolution_clock::now();

	//switch (debugRendering)
	//{
	//case 1:
	//{
	//	visibleBlocks = ChunkRendererTexture::drawChunks(gameMap, startYView, endYView, startXView, endXView);
	//	break;
	//}
	//case 2:
	//{
	//	visibleBlocks = renderer.drawBlocks(startYView, endYView, startXView, endXView);
	//}
	//default:
	//{
	//	visibleBlocks = WorldRendererLegacy::drawBlocks(assetManager, backgroundMap, gameMap, startYView, endYView, startXView, endXView);
	//	break;
	//}
	//}

	//auto end = std::chrono::high_resolution_clock::now();

	//float worldRenderMs = std::chrono::duration<float, std::milli>(end - start).count();

	//std::cout << "Visible Blocks/Chunks: " << visibleBlocks << "\n";
	//std::cout << "World Render Time: " << worldRenderMs << " ms" << "\n";

#pragma endregion


#pragma region render frame and selected block

	//draw selected block
	if (!insideInventoryGrid && !insideCraftingMenu && !insideHotbarMenu)
	{
		DrawTexturePro(
			assetManager.frame,
			{ 0,0,(float)assetManager.frame.width,(float)assetManager.frame.height },
			{ (float)blockX, (float)blockY, 1, 1 },
			{},
			0.f,
			WHITE
		);
	}

	// todo: show this inside inventury menu or not
	DrawTexturePro(
		assetManager.textures,
		getTextureAtlas(creativeSelectedBlock, 0, 32, 32),
		{ (float)blockX, (float)blockY, 1, 1 },
		{},
		0.f,
		{ 255,255,255,127 }
	);

#pragma endregion


#pragma region render entities

	for (auto& e : entityHolder.entities)
	{
		if (DEBUG_MODE == 1)
		{
			DrawRectangleLinesEx(
				e.second->physics.transform.getAABB(),
				.1f,
				PURPLE
			);
		}

		e.second->render(assetManager);
	}

#pragma endregion


#pragma region render & update enemy health bars


	for (auto& e : entityHolder.enemies)
	{
		e->renderHealthBar(assetManager);
		e->updateHealthBar(deltaTime);
	}

#pragma endregion


#pragma region render player

	player.render(assetManager);

#pragma endregion


#pragma region render particles

	renderParticles();

#pragma endregion


#pragma region render structure selection

	// show structure selection
	if (showImgui)
	{
		Rectangle rect;
		rect.x = selectionStart.x;
		rect.y = selectionStart.y;
		rect.width = selectionEnd.x - selectionStart.x;
		rect.height = selectionEnd.y - selectionStart.y;

		rect.width++;
		rect.height++;

		DrawRectangleLinesEx(
			rect,
			0.1f,
			{ 20,101,250,145 }
		);
	}

#pragma endregion


#pragma region render popup text

	drawPopuptext();

#pragma endregion


#pragma region test physics intersect

	//Transform2D test;
	//test.pos = { 20.5, 120.5 };
	//test.w = 1;
	//test.h = 1;

	//Transform2D test2;
	//test2.pos = worldPos;
	//test2.w = 1;
	//test2.h = 1;

	//// point intersect test
	//if (test.intersectPoint(worldPos))
	//{
	//	DrawRectangleLinesEx(test.getAABB(), 0.1, GREEN);
	//}
	//else
	//{
	//	DrawRectangleLinesEx(test.getAABB(), 0.1, BLUE);
	//}

	//// transform vs transform intersect test
	//if (test.intersectTransform(test2))
	//{
	//	DrawRectangleLinesEx(test.getAABB(), 0.1, GREEN);
	//	DrawRectangleLinesEx(test2.getAABB(), 0.1, GREEN);
	//}
	//else
	//{
	//	DrawRectangleLinesEx(test.getAABB(), 0.1, BLUE);
	//	DrawRectangleLinesEx(test2.getAABB(), 0.1, BLUE);
	//}

#pragma endregion


	spriteBatch.end();

	EndMode2D();
	EndTextureMode();

#pragma region draw glow stuff

	// draw only glow stuff here
	BeginTextureMode(glowTexture);
	ClearBackground(BLACK);
	BeginMode2D(camera);

	// render torches here
	// static torch at 29, 59
	DrawCircleGradient(
		29,
		59,
		1.2f,
		Color{ 255,255,0,255 },
		Color{ 0,0,0,0 }
	);

	EndMode2D();
	EndTextureMode();

#pragma endregion


#pragma region glow shader

	// blur glow texture
	BeginTextureMode(blurredGlowTexture);
	BeginShaderMode(assetManager.blurShader);
	ClearBackground(BLACK);
	DrawTextureRec(
		glowTexture.texture,
		Rectangle{ 0, 0, (float)glowTexture.texture.width, (float)-glowTexture.texture.height },
		Vector2{ 0, 0 },
		WHITE
	);
	EndShaderMode();
	EndTextureMode();

#pragma endregion


#pragma region final screen pass

	float depth = player.getPosition().y;

	float caveDarkness = Clamp((depth - 110.0f) / 40.0f, 0.0f, 1.0f);

	float lightingAmount = std::max(isNight(t) ? 1.0f : 0.0f, caveDarkness);

	Color tint = {
		255,
		255,
		255,
		(unsigned char)(lightingAmount * 255.0f)
	};

	// draw scene
	DrawTextureRec(
		sceneTexture.texture,
		{ 0, 0, (float)screenW, -(float)screenH },
		{ 0, 0 },
		WHITE
	);

	// apply darkness only at night/caves
	if (lightingAmount > 0.0f)
	{
		// multiply lighting
		BeginBlendMode(BLEND_MULTIPLIED);

		DrawTexturePro(
			blurredLightTexture.texture,
			Rectangle { // source
				0,
				0,
				(float)blurredLightTexture.texture.width,
				-(float)blurredLightTexture.texture.height
			},
			Rectangle { // dest
				0,
				0,
				(float)screenW,
				(float)screenH
			},
			{ 0,0 }, // origin
			0,      // rotation
			tint   // tint
		);

		EndBlendMode();

		// ADD GLOW/TORCHES ON TOP
		BeginBlendMode(BLEND_ADDITIVE);

		DrawTexturePro(
			blurredGlowTexture.texture,
			{
				0,
				0,
				(float)blurredGlowTexture.texture.width,
				-(float)blurredGlowTexture.texture.height
			},
			{
				0,
				0,
				(float)screenW,
				(float)screenH
			},
			{ 0,0 },
			0,
			WHITE
		);

		EndBlendMode();
	}

#pragma endregion


#pragma region rebuild texture

	if (gameMap.textureNeedsRebuild)
	{
		gameMap.textureNeedsRebuild = false;
		//ChunkRendererTexture::rebuildChunk(assetManager, gameMap);
		renderer.rebuildDirtyChunkRenderData();
	}

#pragma endregion


#pragma region rebuild lighting

	if (gameMap.lightingNeedsRebuild)
	{
		gameMap.lightingNeedsRebuild = false;

		auto start = std::chrono::high_resolution_clock::now();
		recalculateLight(gameMap);
		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "Total Lighting time: " << duration.count() << " us\n" << std::endl;
	}

#pragma endregion


#pragma	region ui

	float w = (float)GetScreenWidth();
	float h = (float)GetScreenHeight();

#pragma region life ui

	Rectangle heartRectangle{};
	heartRectangle.height = h * .05f;
	heartRectangle.width = heartRectangle.height * 5;

	heartRectangle = placeReactangleTopRightCorner(heartRectangle, w);

	//DrawRectangle(heartRectangle.x, heartRectangle.y, heartRectangle.width, heartRectangle.height, RED);

	float damagedLife = std::min((float)player.stats.defensive.maxHealth - player.life, (float)player.stats.defensive.maxHealth);

	// todo: create maxLife variable since we can increase it with items
	for (int i = 0; i < (player.stats.defensive.maxHealth / 10); i++, damagedLife -= 10)
	{
		Rectangle oneHeartRectangle = heartRectangle;
		oneHeartRectangle.width = oneHeartRectangle.height;
		oneHeartRectangle.x += oneHeartRectangle.width * i;


		int x = 0;

		if(damagedLife >= 10)
			x = assetManager.hearts.width / 3;
		else if (damagedLife >= 5)
			x = assetManager.hearts.width * 2 / 3;

		DrawTexturePro(
			assetManager.hearts,
			getTextureAtlas(x, 0, assetManager.hearts.width / 3, assetManager.hearts.height),
			oneHeartRectangle,
			{ 0,0 },
			0.f,
			WHITE
		);
	}

#pragma endregion

#pragma region inventory ui

	//DrawRectangleLinesEx(inventoryRectangle, 2, RED);

	// draw inventory rect
	drawInventoryBackground(
		inventoryRectangle,
		player.inventory,
		insideInventory
	);

	// draw hotbar + inventory
	int slotsToDraw = insideInventory ? player.inventory.rows * player.inventory.columns : player.inventory.columns;
	for (int i = 0; i < slotsToDraw; i++)
	{
		bool isDragged = (i == player.inventory.draggedSlot);

		drawInventorySlotByIndex(
			i,
			isDragged,
			inventoryRectangle,
			player.inventory,
			player,
			assetManager
		);
	}

	// hotbar selection
	for (int i = 0; i < sizeof(hotbarKeys) / sizeof(hotbarKeys[0]); i++)
	{

		if (!showImgui && IsKeyPressed(hotbarKeys[i]))
		{
			// hotbar is selected while player using something, store it to pending and wait for usetimer to run out
			if (player.useTimer > 0)
			{
				pendingHotbarSlot = i;
			}
			else
			{
				player.selectedHotbarSlot = i;
			}
			break;
		}
	}

	// swap item
	int hoveredSlot = getHoveredInventorySlot(
		GetMousePosition(),
		inventoryRectangle,
		player.inventory,
		insideInventory
	);

	// hovered slot out of inventory
	if (hoveredSlot == -1)
	{
		player.inventory.draggedSlot = -1;
	}

	// hotbar menu click to select
	if (insideHotbarMenu && !insideInventory)
	{
		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
		{
			// hotbar is selected while player using something, store it to pending and wait for usetimer to run out
			if (player.useTimer > 0)
			{
				pendingHotbarSlot = hoveredSlot;
			}
			else
			{
				player.selectedHotbarSlot = hoveredSlot;
			}
		}
	}

	// select pending hotbar
	if (pendingHotbarSlot != -1 && player.useTimer <= 0)
	{
		player.selectedHotbarSlot = pendingHotbarSlot;
		pendingHotbarSlot = -1;
	}

	 //equip item from hotbar
	{
		player.selectedHotbarSlot = Clamp(player.selectedHotbarSlot, 0, 8);
		ItemStack& selectedStack = player.inventory.slots[player.selectedHotbarSlot];

		ItemDefinition* item = getItem(selectedStack.itemId);

		if (item && item->category == ItemCategory::ARMOR)
		{
			useArmor(&player, *item, selectedStack, player.selectedHotbarSlot);
		}

		player.heldItem = player.inventory.slots[player.selectedHotbarSlot].itemId;
		player.recalculateStats();
	}
	
	// draw display name of item
	if (hoveredSlot != -1)
	{
		Rectangle slotRect = getInventorySlotRect(hoveredSlot, inventoryRectangle, player.inventory);
		ItemId hoveredItem = player.inventory.slots[hoveredSlot].itemId;
		drawDisplauNameUI(hoveredItem, slotRect);
	}

	if (insideInventoryGrid)
	{
		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
		{
			if (hoveredSlot != -1)
			{
				player.inventory.draggedSlot = hoveredSlot;
			}
		}

		if (IsMouseButtonReleased(MouseButton::MOUSE_BUTTON_LEFT))
		{
			if (hoveredSlot == -1)
			{
				player.inventory.draggedSlot = -1;
			}

			if (hoveredSlot == player.inventory.draggedSlot)
			{
				hoveredSlot = -1;
				player.inventory.draggedSlot = -1;
			}

			if (player.inventory.draggedSlot != -1 && hoveredSlot != -1)
			{
				std::swap(player.inventory.slots[player.inventory.draggedSlot], player.inventory.slots[hoveredSlot]);
				hoveredSlot = -1;
				player.inventory.draggedSlot = -1;
			}
		}

		// draw dragged item at mouse
		if (hoveredSlot != -1 && player.inventory.draggedSlot != -1)
			drawDraggedItem(player.inventory.slots[player.inventory.draggedSlot], assetManager);
	}

#pragma endregion

	// todo: put them in functions - cleanup
#pragma region craft ui


	if (insideCraft)
	{
		float w = (float)GetScreenWidth();
		float h = (float)GetScreenHeight();

		Rectangle craftRectangle = getCraftRectangle(w, h);
		craftRectangle = shrinkRectanglePercentage(craftRectangle, .01f, .01f);
		DrawRectangleLinesEx(craftRectangle, 2, RED);

		Rectangle recipeRect = getRecipeRectangle(w, h, craftRectangle);
		recipeRect = shrinkRectanglePercentage(recipeRect, .01f, .01f);
		DrawRectangleLinesEx(recipeRect, 1, YELLOW);

		Rectangle ingredientRect = getIngredientsRectangle(w, h, craftRectangle, recipeRect);
		ingredientRect = shrinkRectanglePercentage(ingredientRect, .01f, .01f);
		DrawRectangleLinesEx(ingredientRect, 1, GREEN);

		float baseCellSize = recipeRect.width * .7f;

		// RECIPE
		Rectangle oneCellRectangleRecipe = {};
		oneCellRectangleRecipe.width = baseCellSize;
		oneCellRectangleRecipe.height = baseCellSize;
		oneCellRectangleRecipe.x = recipeRect.x + (recipeRect.width - oneCellRectangleRecipe.width) * 0.5f;
		oneCellRectangleRecipe.y = recipeRect.y;
		oneCellRectangleRecipe = shrinkRectanglePercentage(oneCellRectangleRecipe, 0.01f, 0.01f);
		//DrawRectangleLinesEx(oneCellRectangleRecipe, 1, BLUE);

		// INGREDIENT
		Rectangle oneCellRectangleIngredient = {};
		oneCellRectangleIngredient.width = baseCellSize;
		oneCellRectangleIngredient.height = baseCellSize;
		oneCellRectangleIngredient.x = ingredientRect.x;
		oneCellRectangleIngredient.y = ingredientRect.y;
		oneCellRectangleIngredient = shrinkRectanglePercentage(oneCellRectangleIngredient, 0.01f, 0.01f);
		//DrawRectangleLinesEx(oneCellRectangleIngredient, 1, DARKPURPLE);

		auto nearbyStation = getNearbyStation(player.getPosition());
		std::vector<ItemId> availableRecipes = Crafting::getAvailableRecipes(nearbyStation);
		int maxRecipeSize = availableRecipes.size();
		float scroll = GetMouseWheelMove();

		if (scroll < 0)
		{
			if (selectedRecipeIndex < maxRecipeSize - 1)
			{
				selectedRecipeIndex++;
			}
			if (Crafting::startPointer < maxRecipeSize - Crafting::maxRecipeToShow)
				Crafting::startPointer++;
		}
		else if (scroll > 0)
		{
			if (selectedRecipeIndex > 0)
			{
				selectedRecipeIndex--;
			}
			if (Crafting::startPointer > 0)
				Crafting::startPointer--;
		}

		selectedRecipeIndex = Clamp(selectedRecipeIndex, 0, maxRecipeSize - 1);

		if (IsKeyPressed(KEY_ENTER))
		{
			ItemId selectedItemType = availableRecipes[selectedRecipeIndex];
			auto nearbyStation = getNearbyStation(player.getPosition());
			Crafting::CraftCheckResult craftingResult = Crafting::canCraft(player.inventory.slots, selectedItemType, nearbyStation);
			bool canCraft = craftingResult.canCraft;

			if (canCraft)
			{
				Crafting::craft(player.inventory, selectedItemType);
			}
		}

		ItemId selectedItemType = availableRecipes[selectedRecipeIndex];

		int padding = 10;

		for (int i = Crafting::startPointer; i < std::min(Crafting::startPointer + Crafting::maxRecipeToShow, maxRecipeSize); i++)
		{
			// check nearby station
			auto nearbyStation = getNearbyStation(player.getPosition());

			ItemId itemType = availableRecipes[i];
			Crafting::CraftCheckResult craftingResult = Crafting::canCraft(player.inventory.slots, itemType, nearbyStation);
			bool canCraft = craftingResult.canCraft;

			// item rectangle
			Rectangle rr = oneCellRectangleRecipe;
			rr.y += (i - Crafting::startPointer) * (oneCellRectangleRecipe.height + padding);
			rr = shrinkRectanglePercentage(rr, .1f, .1f);

			// item and bg colors
			Color bg;
			Color itemColor;

			if (CheckCollisionPointRec(GetMousePosition(), rr))
			{
				selectedRecipeIndex = i;

				drawDisplauNameUI(selectedItemType, rr);

				// craft item if clicked
				if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && canCraft)
				{
					Crafting::craft(player.inventory, selectedItemType);
				}
			}

			if (selectedRecipeIndex == i)
			{
				// Selected item
				rr = enlargeRectanglePercentage(rr, .2f, .2f);
				bg = { 255, 220, 20, 255 };
				itemColor = canCraft ? WHITE : ColorAlpha(WHITE, 0.55f);
			}
			else
			{
				// Normal slot
				bg = canCraft ? Color{ 48, 125, 255, 255 } : Color{ 28, 55, 110, 255 };
				itemColor = canCraft ? WHITE : ColorAlpha(WHITE, 0.4f);
			}

			DrawRectangleRounded(rr, .3f, 6, bg);

			rr = shrinkRectanglePercentage(rr, .4f, .4f);

			auto atlas = getTextureCoordinatesForItemType(itemType);
			Texture2D tex = getTextureForItemType(itemType, assetManager);
			DrawTexturePro(
				tex,
				atlas,
				rr,
				{ 0,0 },
				0.f,
				itemColor
			);

			if (itemType != selectedItemType) continue;

			for (int j = 0; j < Recipes::all[itemType].ingredients.size(); j++)
			{
				Rectangle ri = oneCellRectangleIngredient;
				ri.x += j * oneCellRectangleIngredient.width;
				ri.y += (i - Crafting::startPointer) * (oneCellRectangleRecipe.height + padding);
				ri = shrinkRectanglePercentage(ri, .3f, .3f);
				std::vector<ItemStack> selectedItemIngredients = Recipes::all[itemType].ingredients;
				int ingredient = selectedItemIngredients[j].itemId;
				bool hasEnoughIngredients = Crafting::hasEnoughIngredients(player.inventory.slots, selectedItemIngredients[j]);

				DrawRectangleRounded(ri, .3f, 1, { 48, 125, 255, 255 }); // blue color

				ri = shrinkRectanglePercentage(ri, .25f, .25f);

				auto atlas = getTextureCoordinatesForItemType(ingredient);
				Texture2D tex = getTextureForItemType(ingredient, assetManager);
				DrawTexturePro(
					tex,
					atlas,
					ri,
					{ 0,0 },
					0.f,
					itemColor
				);

				std::string str = std::to_string(selectedItemIngredients[j].count);
				Vector2 textPos =
				{
					ri.x + ri.width * 0.5f,
					ri.y + ri.height * 0.85f
				};
				Vector2 textSize = MeasureTextEx(GetFontDefault(), str.c_str(), 10.f, 1.f);

				DrawTextPro(
					GetFontDefault(),
					str.c_str(),
					textPos,
					{ textSize.x / 2.f, textSize.y / 2.f },
					0.f,
					10.f,
					1.f,
					hasEnoughIngredients ? Color{ 255, 255, 255, 200 } : RED
				);

				if (CheckCollisionPointRec(GetMousePosition(), ri))
				{
					drawDisplauNameUI(selectedItemIngredients[j].itemId, ri);
				}
			}
		}
	}

#pragma endregion

#pragma region world clock

	WorldTimeClock clock = getWorldTimeClock(t);

	std::string hour = std::to_string(clock.hh);
	if (clock.hh < 10)
		hour = "0" + hour;

	std::string minute = std::to_string(clock.mm);
	if (clock.mm < 10)
		minute = "0" + minute;

	std::string strClock = hour + " : " + minute;

	DrawTextEx(
		GetFontDefault(),
		strClock.c_str(),
		{ 20,40 },
		20,
		5,
		GREEN
	);

	DrawTextEx(
		GetFontDefault(),
		phase_to_str(skyData.phase),
		{ 120, 40 },
		20,
		5,
		GREEN
	);

#pragma endregion

#pragma endregion


#pragma region ImGui Madness

	if (showImgui)
	{
		ImGui::Begin("Game Control");

		ImGui::Text("Debug Tile Cords: ");
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
		ImGui::Text("%.2f, %.2f", mousePos.x, mousePos.y);

		std::string s = "Projectiles: ";
		s += std::to_string(entityHolder.projectiles.size());
		ImGui::Text(s.c_str());

		ImGui::Separator();

		std::string enemyCount = "Enemy Count: ";
		enemyCount += std::to_string(entityHolder.enemies.size());
		ImGui::Text(enemyCount.c_str());

		ImGui::Separator();

		std::string entityCount = "Entity Count: ";
		entityCount += std::to_string(entityHolder.entities.size());
		ImGui::Text(entityCount.c_str());

		ImGui::Separator();

		ImGui::Text("Player Stats");
		ImGui::Text("MaxHealth:   %d", player.stats.defensive.maxHealth);
		ImGui::Text("Damage:      %d", player.stats.offensive.damage);
		ImGui::Text("Armor:       %d", player.stats.defensive.armor);
		ImGui::Text("Crit Chance: %d", player.stats.offensive.critChance);
		ImGui::Text("Crit Damage: %d", player.stats.offensive.critDamage);

		ImGui::Separator();

		ImGui::SliderFloat("Camera Zoom: ", &camera.zoom, 10, 150);
		//ImGui::SliderFloat("Camera Speed: ", &CAMERA_SPEED, 5, 100);

		ImGui::Checkbox("Creative", &creative);

		if (ImGui::Button("Spawn slime"))
		{
			spawnEnemyHelper<Slime>({ 18,60 });
		}

		ImGui::InputText(("Texture Pack"), texturePackName, sizeof(texturePackName));
		if (ImGui::Button("Load Texture"))
		{
			assetManager.loadTexturePack(texturePackName);
		}

		ImGui::Separator();

		if (ImGui::Button("Copy"))
		{
			copyStructure.copyFromMap(gameMap, selectionStart, selectionEnd);
		}

		ImGui::InputText("File name", saveName, sizeof(saveName));

		if (ImGui::Button("Save to file"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += saveName;
			path += ".bin";

			saveBlockDataToFile(
				copyStructure.mapData,
				copyStructure.w,
				copyStructure.h,
				path.c_str()
			);
		}

		if (ImGui::Button("Load from file"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += saveName;
			path += ".bin";

			bool d = loadBlockDataFromFile(
				copyStructure.mapData,
				copyStructure.w,
				copyStructure.h,
				path.c_str()
			);

			printf("load: %d", d);
		}

		ImGui::Separator();

		ImGui::SliderFloat("Master Volume", &getSettings().masterVolume, 0, 1);
		ImGui::SliderFloat("Sound Volume", &getSettings().soundsVolume, 0, 1);
		ImGui::SliderFloat("Music Volume", &getSettings().musicVolume, 0, 1);

		if (ImGui::Button("Save Settings"))
		{
			saveSettings();
		}
		if (ImGui::Button("Load Settings"))
		{
			loadSettings();
		}

		if (ImGui::Button("Play Sound"))
		{
			Audio::playSound(Audio::placeBlock);
		}

		if (ImGui::Button("Play Forest Music"))
		{
			Audio::playMusic(Audio::musicForest);
		}

		if (ImGui::Button("Play Desert Music"))
		{
			Audio::playMusic(Audio::musicDesert);
		}

		ImGui::Separator();

		for (int i = 0; i < Items::LAST_BLOCK; i++)
		{
			auto atlas = getTextureAtlas(i, 0, 32, 32);
			atlas.x /= assetManager.textures.width;
			atlas.width /= assetManager.textures.width;
			atlas.y /= assetManager.textures.height;
			atlas.height /= assetManager.textures.height;

			ImGui::PushID(i);

			ImTextureID tex = (ImTextureID)(intptr_t)assetManager.textures.id;
			if (ImGui::ImageButton(
				tex,
				{ 35,35 },
				{ atlas.x,atlas.y },
				{ atlas.x + atlas.width,atlas.y + atlas.height }
			))
			{
				creativeSelectedBlock = i;
			}

			ImGui::PopID();

			if (i % 10 != 0)
			{
				ImGui::SameLine();
			}
		}

		ImGui::End();
	}

#pragma endregion


#pragma region display fps

	DrawFPS(20, 20);

#pragma endregion

	return true;
}

void Gameplay::closeGame(AssetManager& assetManager) const
{
	UnloadRenderTexture(lightMask);
	UnloadRenderTexture(sceneTexture);
	UnloadRenderTexture(glowTexture);
	UnloadRenderTexture(blurredLightTexture);
	UnloadRenderTexture(blurredGlowTexture);

	UnloadShader(assetManager.blurShader);
	UnloadShader(assetManager.bloomShader);
	UnloadShader(assetManager.flashShader);
}
