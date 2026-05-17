#include "gameplay.h"
#include <iostream>
#include <imgui.h>

#include <ui.h>
#include <audio.h>
#include <settings.h>
#include <helper.h>
#include <saveMap.h>
#include <worldGenerator.h>

#include <shake.h>
#include <entities/droppedItem.h>
#include <entities/enemies/enemy.h>
#include <entities/enemies/slime.h>
#include <entities/enemies/desetSlime.h>
#include <entities/enemies/zombie.h>

#include <items/item.h>
#include <items/itemUse.h>

#include <combat/melee.h>
#include "combat/tool.h"

#include "ui/popupText.h"



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

bool Gameplay::isNight(float t)
{
	return (t >= 0.5f && t < 0.9f);
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

void Gameplay::spawnSlime(Vector2 position)
{
	Slime slime;
	slime.teleport(position);

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.entities[id] = std::make_unique<Slime>(slime);
}

void Gameplay::spawnDesertSlime(Vector2 position)
{
	DesertSlime desertSlime;
	desertSlime.teleport(position);

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.entities[id] = std::make_unique<DesertSlime>(desertSlime);
}

void Gameplay::spawnZombie(Vector2 position)
{
	Zombie zombie;
	zombie.teleport(position);

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.entities[id] = std::make_unique<Zombie>(zombie);
}

void Gameplay::spawnDroppedItem(Vector2 positon, int type)
{
	DroppedItem droppedItem;
	droppedItem.teleport(positon);
	droppedItem.itemType = type;
	droppedItem.physics.velocity.y = -3.f;

	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
	entityHolder.entities[id] = std::make_unique<DroppedItem>(droppedItem);
}

Rectangle Gameplay::getInventoryRectangle(float w, float h)
{
	Rectangle inventoryRectangle = {};

	inventoryRectangle.height = h * .3f;
	inventoryRectangle.width = inventoryRectangle.height * 3;

	float maxWidth = w * .9f;
	if (inventoryRectangle.width > maxWidth)
	{
		float scaleFactor = maxWidth / inventoryRectangle.width;
		inventoryRectangle.height *= scaleFactor;
		inventoryRectangle.width *= scaleFactor;
	}

	inventoryRectangle = placeRectangleTopLeft(inventoryRectangle);

	inventoryRectangle.x += w * .01f;
	inventoryRectangle.y += h * .01f;

	return inventoryRectangle;
}

Rectangle Gameplay::getCraftRectangle(float w, float h)
{
	Rectangle craftRectangle = {};

	// Base size
	craftRectangle.width = w * 0.15f;
	craftRectangle.height = h * 0.35f;

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

Rectangle Gameplay::getRecipeRectangle(
	float w,
	float h,
	Rectangle craftRectangle)
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

Rectangle Gameplay::getIngredientsRectangle(
	float w,
	float h,
	Rectangle craftRectangle,
	Rectangle recipeRectangle)
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

bool Gameplay::init()
{
	double loadStart = GetTime();
	int w = 900, h = 500;

	backgroundMap.create(w, h);
	generateWorld(gameMap);
	registerItems();

	camera.target = { 20, 120 };
	camera.rotation = 0.f;
	camera.zoom = CAMERA_ZOOM;

	player.teleport({ 20, 60 });
	player.physics.transform.w = 0.9f;
	player.physics.transform.h = 1.8f;

	// Light mask render texture
	//lightMask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	camFollow.init(1.5f, .74f, 1.f, player.getPosition());

	//spawnZombie({ 25,60 });
	maxEnemyCount = 5;

	inventory.storeItem(ItemStack{ Items::woodAxe, 1 });

	lifetime = 3;

	std::ranlux24_base rng(std::random_device{}());
	float randStartTime = getRandomFloat(rng, 0.375f, 0.75f);
	worldTime = randStartTime * FULL_DAY_LENGTH;
	double loadEnd = GetTime();
	TraceLog(LOG_INFO, "Load time: %.3f seconds", loadEnd - loadStart);
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
	updateCameraShake(deltaTime);
	updateParticles(particles, deltaTime);
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


#pragma region clear background

	ClearBackground(skyData.skyColor);

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

	// ← snapshot HERE, before any physics or input runs
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

	auto updateEntityPhysics = [&](auto& entity, bool applyGravity = true)
		{
			if (applyGravity) { entity.physics.applyGravity(); }
			entity.physics.updateForces(deltaTime);
			entity.physics.resolveConstrains(gameMap);
			entity.physics.updateFinal();
		};

	updateEntityPhysics(player, false);

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
		auto b = gameMap.getBlockSafe(playerPosLeft.x, playerPosLeft.y);
		if (b && b->type != Block::air)
		{
			blockType = b->type;
		}
		else
		{
			// check collider right
			Vector2 playerPosRight = player.physics.transform.getBottomRight();
			auto b = gameMap.getBlockSafe(playerPosRight.x, playerPosRight.y);
			if (b && b->type != Block::air) blockType = b->type;
		}

		// spawn your particles here
		auto rightParticles = spawnParticles(
			playerPos,
			rng,
			blockType,
			(player.numberOfParticlesOnLand * int(landingVelocity)) / 2,
			player.physics.transform.w,
			45
		);

		auto leftParticles = spawnParticles(
			playerPos,
			rng,
			blockType,
			(player.numberOfParticlesOnLand * int(landingVelocity)) / 2,
			player.physics.transform.w,
			-45
		);
		particles.insert(particles.end(), leftParticles.begin(), leftParticles.end());
		particles.insert(particles.end(), rightParticles.begin(), rightParticles.end());
	}

	player.update(deltaTime, EntityUpdateData
		{
			player.getPosition(),
			rng,
			entityHolder,
			inventory,
			gameMap
		}
	);
#pragma endregion


#pragma region clamp camera

	// clamp camera
	{
		float zoom = camera.zoom;

		float screenWidth = GetScreenWidth();
		float screenHeight = GetScreenHeight();

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

	bool shouldApplyGravity = true;
	float groundDistance = 0;
	bool shouldStepUp = false;

	// update all entities
	for (auto it = entityHolder.entities.begin(); it != entityHolder.entities.end();)
	{
		bool shouldKill = false;

		// dropped item specific logic
		if (it->second->getEntityType() == EntityType::EntityType_DroppedItem)
		{
			if (it->second->physics.transform.intersectTransform(player.physics.transform))
			{
				DroppedItem* d = reinterpret_cast<DroppedItem*>(it->second.get());
				ItemStack itemStack
				{
					d->itemType,
					d->itemCounter
				};
				shouldKill = !inventory.storeItem(itemStack);
			}
		}

		EntityUpdateData entityUpdateData
		{
			player.getPosition(),
			rng,
			entityHolder,
			inventory,
			gameMap,
			it->first
		};


		if (!it->second->update(deltaTime, entityUpdateData))
		{
			shouldKill = true;
		}

		if (shouldKill)
		{
			// erase returns next valid iterator
			it = entityHolder.entities.erase(it);
		}
		else
		{
			// physics
			updateEntityPhysics(*it->second, shouldApplyGravity);
			++it;
		}
	}

#pragma endregion


#pragma region handle inventory

	for (int i = 0; i < inventory.slots.size(); i++)
	{
		if (inventory.slots[i].itemId != 0 && inventory.slots[i].count <= 0)
		{
			inventory.removeItem(i);

			// clear selection
			creativeSelectedBlock = 0;
			player.heldItem = 0;
		}
	}

#pragma endregion


#pragma region handle input

	bool insideInventoryMenu = false;
	Rectangle inventoryRectangle = getInventoryRectangle(GetScreenWidth(), GetScreenHeight());

	if (insideInventory && CheckCollisionPointRec(GetMousePosition(), inventoryRectangle))
	{
		insideInventoryMenu = true;
	}

	bool insideCraftingMenu = false;
	Rectangle craftRectangle = getCraftRectangle(GetScreenWidth(), GetScreenHeight());

	if (insideCraft && CheckCollisionPointRec(GetMousePosition(), craftRectangle))
	{
		insideCraftingMenu = true;
	}

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (creativeSelectedBlock < 0) { creativeSelectedBlock = 0; }
	if (creativeSelectedBlock >= Block::BLOCKS_COUNT) { creativeSelectedBlock = Block::BLOCKS_COUNT - 1; }

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
		if (!insideInventoryMenu && !insideCraftingMenu)
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

		if (!insideInventoryMenu && !insideCraftingMenu)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				// attack and tool done
				// todo consumable and place block
				useItem(player, ItemStack{ player.heldItem, 1 }, worldPos);
			}
		}

		if (!insideInventoryMenu && !insideCraftingMenu)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				float magnitude = Vector2Distance(player.getPosition(), worldPos);
				if (magnitude <= 5 || creative)
				{
					// place block only if its withing reach, 
					// no existing blocks and 
					// adjecent block exists
					auto b = gameMap.getBlockSafe(blockX, blockY);
					if (b && b->type == Block::air && gameMap.isAdjacentBlock(blockX, blockY))
					{
						for (auto& i : inventory.slots)
						{
							// check inventory to see if we have same type of item and have more than 0
							if (creativeSelectedBlock == i.itemId && i.count > 0)
							{
								b->type = i.itemId;
								i.count -= 1;
							}
						}
					}
				}
			}
		}
	}

#pragma endregion


#pragma region handle melee attacks

	MeleeHitResult meleeResult = updateMeleeAttacks(deltaTime, entityHolder.entities);

	if (meleeResult.hit)
	{
		triggerCameraShake(.1f, .15f);
		spawnPopupText(
			meleeResult.positon,
			Vector2{ .1f, .1f },
			std::to_string(meleeResult.damage),
			1,
			.4f,
			-1.f,
			WHITE
		);
	}

#pragma endregion


#pragma region handle tool swing

	ToolHitResult swingResult = updateToolSwing(deltaTime, gameMap);

	if (swingResult.hit)
	{
		triggerShake((int)swingResult.position.x, (int)swingResult.position.y);
		auto newParticles = spawnParticles({ swingResult.position.x, swingResult.position.y }, rng, swingResult.type, 10);
		particles.insert(particles.end(), newParticles.begin(), newParticles.end());

		if (swingResult.broke)
			spawnDroppedItem({ std::floor(swingResult.position.x) + 0.5f, swingResult.position.y + 0.5f }, swingResult.type);

		spawnPopupText(
			swingResult.position,
			Vector2{ 0, .1f },
			std::to_string(swingResult.power),
			1,
			.2f,
			-1.f,
			WHITE
		);
	}

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

		background.setBackground(backgroundType);

		background.draw(
			deltaTime,
			assetManager,
			camera,
			{ (float)gameMap.w, (float)gameMap.h }
		);
	}

#pragma endregion

	BeginMode2D(camera);

#pragma region draw world

	Vector2 topLeftView = GetScreenToWorld2D({ 0,0 }, camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, camera);

	int startXView = (int)floorf(topLeftView.x - 1);
	int endXView = (int)floorf(bottomRightView.x + 1);
	int startYView = (int)floorf(topLeftView.y - 1);
	int endYView = (int)floorf(bottomRightView.y + 1);

	startXView = Clamp((float)startXView, 0.f, (float)gameMap.w - 1);
	endXView = Clamp((float)endXView, 0.f, (float)gameMap.w - 1);

	startYView = Clamp((float)startYView, 0.f, (float)gameMap.h - 1);
	endYView = Clamp((float)endYView, 0.f, (float)gameMap.h - 1);

	enemySpawner.enemySpawnTimer -= deltaTime;

	int enemyCount = 0;

	for (auto& [id, entity] : entityHolder.entities)
	{
		if (entity->getEntityType() == EntityType::EntityType_Enemy)
		{
			enemyCount++;
		}
	}

	if (enemySpawner.enemySpawnTimer <= 0 && enemyCount < maxEnemyCount)
	{
		spawnEnemy(entityHolder, gameMap, rng, startXView, endXView, startYView, endYView);
		enemySpawner.enemySpawnTimer = 2;
	}

	for (int y = startYView; y <= endYView; y++)
	{
		for (int x = startXView; x <= endXView; x++)
		{
			float size = 1;
			float posX = x * size;
			float posY = y * size;

			int atlasX = 0;

			auto& bb = backgroundMap.getBlockUnsafe(x, y);

			if (bb.type != Block::air)
			{
				atlasX = bb.type;

				if (bb.variation == -1)
				{
					bb.variation = rand() % 4;
				}

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(atlasX, bb.variation, 32, 32),
					{ posX,posY,size,size }, //dest
					{ 0,0 }, //origin (top-left)
					0.f,     //rotation
					WHITE    //tint
				);
			}

			auto& b = gameMap.getBlockUnsafe(x, y);


			if (b.type != Block::air)
			{
				atlasX = b.type;

				if (b.variation == -1)
				{
					b.variation = rand() % 4;
				}

				Vector2 shake = getShakeOffset(x, y);

				float drawX = posX + shake.x;
				float drawY = posY + shake.y;

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(atlasX, b.variation, 32, 32), //source (in sprite)
					{ drawX,drawY,size,size }, //dest
					{ 0,0 }, //origin (top-left)
					0.f,     //rotation
					{ WHITE }

				);
			}
		}
	}

#pragma endregion


#pragma region draw frame and selected block

	//draw selected block
	if (!insideInventoryMenu && !insideCraftingMenu)
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
		e.second->render(assetManager);
		//DrawRectangleLines(
		//	e.second->physics.transform.pos.x,
		//	e.second->physics.transform.pos.y,
		//	e.second->physics.transform.w,
		//	e.second->physics.transform.h,
		//	PURPLE
		//);

		if (e.second->getEntityType() == EntityType_Enemy)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(e.second.get());
			enemy->renderHealthBar(assetManager);
			enemy->updateHealthBar(deltaTime);
		}
	}

#pragma endregion


#pragma region render player

	player.render(assetManager);

#pragma endregion


#pragma region render particles

	renderParticles(particles);

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
			0.1,
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

	EndMode2D();

#pragma region day/night cycle render

	DrawRectangle(
		0,
		0,
		GetScreenWidth(),
		GetScreenHeight(),
		Fade(ambientColor, darkness)
	);

#pragma endregion


#pragma region lighting

	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	if (lightMask.id == 0 || screenW != lastScreenWidth || screenH != lastScreenHeight)
	{
		// Free old one if exists
		if (lightMask.id != 0)
		{
			UnloadRenderTexture(lightMask);
		}

		// Create new render texture
		lightMask = LoadRenderTexture(screenW, screenH);

		lastScreenWidth = screenW;
		lastScreenHeight = screenH;
	}

	// Apply cave lighting ONLY when underground
	if (player.getPosition().y > 130)
	{

		// Convert player world pos -> screen pos
		Vector2 playerScreen = GetWorldToScreen2D(
			player.physics.transform.getCenter(),
			camera
		);

		// Update light mask in screen space
		BeginTextureMode(lightMask);
		ClearBackground(BLACK);
		DrawCircleGradient(playerScreen.x, playerScreen.y, 600, WHITE, BLACK);
		EndTextureMode();

		// Overlay mask on screen
		BeginBlendMode(BLEND_MULTIPLIED);
		DrawTextureRec(
			lightMask.texture,
			{ 0, 0, (float)screenW, (float)-screenH }, // negative H = flip Y
			{ 0, 0 },
			WHITE
		);
		EndBlendMode();
	}

#pragma endregion


#pragma	region ui

	float w = GetScreenWidth();
	float h = GetScreenHeight();

#pragma region life ui

	Rectangle heartRectangle{};
	heartRectangle.height = h * .05f;
	heartRectangle.width = heartRectangle.height * 5;

	heartRectangle = placeReactangleTopRightCorner(heartRectangle, w);

	//DrawRectangle(heartRectangle.x, heartRectangle.y, heartRectangle.width, heartRectangle.height, RED);

	for (int i = 0; i < 5; i++)
	{
		Rectangle oneHeartRectangle = heartRectangle;
		oneHeartRectangle.width = oneHeartRectangle.height;
		oneHeartRectangle.x += oneHeartRectangle.width * i;

		DrawTexturePro(
			assetManager.hearts,
			getTextureAtlas(0, 0, assetManager.hearts.width / 3, assetManager.hearts.height),
			oneHeartRectangle,
			{ 0,0 },
			0.f,
			WHITE
		);
	}

#pragma endregion

#pragma region inventory ui

	if (insideInventory)
	{
		Rectangle inventoryRectangle = getInventoryRectangle(w, h);

		DrawRectangle(
			inventoryRectangle.x,
			inventoryRectangle.y,
			inventoryRectangle.width,
			inventoryRectangle.height,
			{ 100,100,100,100 }
		);

		inventoryRectangle = shrinkRectanglePercentage(inventoryRectangle, .01f, .01f);

		Rectangle oneCellRectangle{};
		oneCellRectangle.height = inventoryRectangle.height / 3;
		oneCellRectangle.width = oneCellRectangle.height;
		oneCellRectangle.x = inventoryRectangle.x;
		oneCellRectangle.y = inventoryRectangle.y;

		for (int i = 0; i < inventory.rows; i++)
		{
			for (int j = 0; j < inventory.cols; j++)
			{
				int index = j * inventory.rows + i;
				Rectangle r = oneCellRectangle;
				r.x += i * oneCellRectangle.width;
				r.y += j * oneCellRectangle.height;

				r = shrinkRectanglePercentage(r, .1f, .1f);

				Color c = { 180,180,200,240 };

				if (CheckCollisionPointRec(GetMousePosition(), r))
				{
					c = { 220,250,220,250 };

					// equip item on left click
					if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
					{
						if (player.selectedHotbarSlot == index)
						{
							player.heldItem = 0;
							player.selectedHotbarSlot = -1;
						}
						else
						{
							if (!inventory.isHotbarSlot(index)) continue;

							player.selectedHotbarSlot = index;
							player.heldItem = inventory.slots[index].itemId;
						}
					}
				}
				else
				{
					c = { 180,180,200,240 };
				}

				// selected override
				if (player.selectedHotbarSlot == index)
				{
					c = { 255,230,80,255 };
				}

				DrawTexturePro(
					assetManager.frame,
					getTextureAtlas(0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height),
					r,
					{ 0,0 },
					0.f,
					c
				);

				// display item from inventory
				auto atlas = getTextureCoordinatesForItemType(inventory.slots[index].itemId);
				Texture2D tex = getTextureForItemType(inventory.slots[index].itemId, assetManager);

				DrawTexturePro(
					tex,
					atlas,
					shrinkRectanglePercentage(r, .3f, .3f),
					{ 0,0 },
					0.f,
					c
				);

				if (inventory.slots[index].count != 0 && !isItem(inventory.slots[index].itemId))
				{
					Vector2 textPos =
					{
						r.x + r.width * 0.5f,
						r.y + r.height * 0.75f
					};
					std::string str = std::to_string(inventory.slots[index].count);
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
	}

#pragma endregion

#pragma region craft ui


	if (insideCraft)
	{
		float w = GetScreenWidth();
		float h = GetScreenHeight();

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
		oneCellRectangleRecipe = shrinkRectanglePercentage(oneCellRectangleRecipe, 0.1f, 0.1f);
		//DrawRectangleLinesEx(oneCellRectangleRecipe, 1, BLUE);

		// INGREDIENT
		Rectangle oneCellRectangleIngredient = {};
		oneCellRectangleIngredient.width = baseCellSize;
		oneCellRectangleIngredient.height = baseCellSize;
		oneCellRectangleIngredient.x = ingredientRect.x;
		oneCellRectangleIngredient.y = ingredientRect.y;
		oneCellRectangleIngredient = shrinkRectanglePercentage(oneCellRectangleIngredient, 0.1f, 0.1f);
		//DrawRectangleLinesEx(oneCellRectangleIngredient, 1, DARKPURPLE);

		int maxRecipeSize = inventory.visibleRecipes.size();
		float scroll = GetMouseWheelMove();

		if (scroll < 0)
		{
			if (selectedRecipeIndex < maxRecipeSize - 1)
			{
				selectedRecipeIndex += 1;
				selectedRecipeIndex = Clamp(selectedRecipeIndex, 0, maxRecipeSize);
			}
		}
		else if (scroll > 0)
		{
			if (selectedRecipeIndex > 0)
			{
				selectedRecipeIndex -= 1;
				selectedRecipeIndex = Clamp(selectedRecipeIndex, 0, maxRecipeSize);
			}
		}

		if (IsKeyPressed(KEY_ENTER))
		{
			int selectedItemType = inventory.visibleRecipes[selectedRecipeIndex];
			bool canCraft = inventory.canCraft(selectedItemType);

			if (canCraft)
			{
				int item = inventory.craft(selectedItemType);
				spawnDroppedItem(player.getPosition(), item);
			}
		}

		int padding = 10;

		for (int i = 0; i < maxRecipeSize; i++)
		{
			int itemType = inventory.visibleRecipes[i];
			bool canCraft = inventory.canCraft(itemType);
			int selectedItemType = inventory.visibleRecipes[selectedRecipeIndex];

			// item rectangle
			Rectangle rr = oneCellRectangleRecipe;
			rr.y += i * (oneCellRectangleRecipe.height + padding);
			rr = shrinkRectanglePercentage(rr, .1f, .1f);

			// item and bg colors
			Color bg;
			Color itemColor;

			if (CheckCollisionPointRec(GetMousePosition(), rr))
			{
				selectedRecipeIndex = i;

				// craft item if clicked
				if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && canCraft)
				{
					int item = inventory.craft(selectedItemType);
					spawnDroppedItem(player.getPosition(), item);
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

			for (int j = 0; j < inventory.receipes[itemType].ingredients.size(); j++)
			{
				Rectangle ri = oneCellRectangleIngredient;
				ri.x += j * oneCellRectangleIngredient.width;
				ri.y += i * oneCellRectangleIngredient.height + padding;
				ri = shrinkRectanglePercentage(ri, .3f, .3f);
				std::vector<ItemStack> selectedItemIngredients = inventory.receipes[itemType].ingredients;
				int ingredient = selectedItemIngredients[j].itemId;
				bool hasEnoughIngredients = inventory.hasEnoughIngredients(selectedItemIngredients[j]);

				DrawRectangleRounded(ri, .3f, 1.f, { 48, 125, 255, 255 }); // blue color

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

		ImGui::InputFloat("lifetime", &lifetime);

		ImGui::SliderFloat("Camera Zoom: ", &camera.zoom, 10, 150);
		//ImGui::SliderFloat("Camera Speed: ", &CAMERA_SPEED, 5, 100);

		ImGui::Checkbox("Creative", &creative);

		if (ImGui::Button("Spawn slime"))
		{
			spawnSlime({ 18,60 });
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

		for (int i = 0; i < Block::BLOCKS_COUNT; i++)
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

void Gameplay::closeGame() const
{
	UnloadRenderTexture(lightMask);
}
