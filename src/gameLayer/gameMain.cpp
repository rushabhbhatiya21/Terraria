#include <iostream>
#include <fstream>
#include <string>
#include <raylib.h>
#include <raymath.h>

#include <imgui.h>
#include <rlImGui.h>

#include "gameMain.h"
#include <assetManager.h>
#include <editorState.h>
#include <gameMap.h>
#include <helper.h>
#include <drawBackground.h>
#include <worldGenerator.h>
#include <randomStuff.h>

#include <structure.h>
#include <saveMap.h>
#include <physics.h>
#include <entityHolder.h>
#include <shake.h>

#include <items.h>
#include <player.h>
#include <inventory.h>
#include <entities/droppedItem.h>
#include <entities/slime.h>
#include <entities/desetSlime.h>
#include <entities/evilEye.h>
#include <entities/zombie.h>

#include <itemData.h>

#include <audio.h>
#include <settings.h>


#pragma region global variables

struct GameData
{
	// core
	GameMap gameMap = {};
	GameMap backgroundMap = {}; 
	Camera2D camera = {};
	DrawBackground background;
	Inventory inventory;

	int creativeSelectedBlock = Block::air;

	// entities
	Player player;
	EntityHolder entityHolder;

	// crafitng
	int maxCraftSlots = 2;
	std::vector<int> craftSlots;

	// texture pack
	char texturePackName[128] = "default";

	// structure save
	Structure copyStructure;
	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};
	char saveName[100] = {};

} gameData;

AssetManager assetManager;

bool showImgui = false;
bool showCraftUI = false;

#pragma endregion


#pragma region helper imgui functions

void spawnSlime(Vector2 position)
{
	Slime slime;
	slime.teleport(position);

	auto id = gameData.entityHolder.idHolder.getEntityIdAndIncreament();
	gameData.entityHolder.entities[id] = std::make_unique<Slime>(slime);
}

void spawnDesertSlime(Vector2 position)
{
	DesertSlime desertSlime;
	desertSlime.teleport(position);

	auto id = gameData.entityHolder.idHolder.getEntityIdAndIncreament();
	gameData.entityHolder.entities[id] = std::make_unique<DesertSlime>(desertSlime);
}

void spawnEvilEye(Vector2 position)
{
	EvilEye evilEye;
	evilEye.teleport(position);

	auto id = gameData.entityHolder.idHolder.getEntityIdAndIncreament();
	gameData.entityHolder.entities[id] = std::make_unique<EvilEye>(evilEye);
}

void spawnZombie(Vector2 position)
{
	Zombie zombie;
	zombie.teleport(position);

	auto id = gameData.entityHolder.idHolder.getEntityIdAndIncreament();
	gameData.entityHolder.entities[id] = std::make_unique<Zombie>(zombie);
}

void spawnDroppedItem(Vector2 positon, int type)
{
	DroppedItem droppedItem;
	droppedItem.teleport(positon);
	droppedItem.itemType = type;

	auto id = gameData.entityHolder.idHolder.getEntityIdAndIncreament();
	gameData.entityHolder.entities[id] = std::make_unique<DroppedItem>(droppedItem);
}

#pragma endregion


bool initGame()
{
	Audio::init();
	assetManager.loadAll();
	loadSettings();

	gameData.craftSlots.resize(gameData.maxCraftSlots);

	int w = 900, h = 500;

	gameData.backgroundMap.create(w, h);
	generateWorld(gameData.gameMap);

	gameData.camera.target = { 20, 120 };
	gameData.camera.rotation = 0.f;
	gameData.camera.zoom = 50.f;

	gameData.player.teleport({ 20, 60 });
	gameData.player.physics.transform.w = 0.9f;
	gameData.player.physics.transform.h = 1.8f;

	//spawnZombie({ 18,60 });

	//spawnDroppedItem({ 25,60 }, 6001);

	return true;
}

bool updateGame()
{
	Audio::update();
	updateSettings();

#pragma region delta time

	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5.f) deltaTime = 1 / 5.f;

#pragma endregion


#pragma region reset times and updates

	if (gameData.player.timeAfterMine > 0)
		gameData.player.timeAfterMine -= deltaTime;

	if (gameData.player.timeAfterAttack > 0)
		gameData.player.timeAfterAttack -= deltaTime;

	updateShake(deltaTime);
	updateCameraShake(deltaTime);

#pragma endregion


#pragma region set camera offset for shake

	gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };

	Vector2 camOffset = { 0, 0 };

	if (camShake.time > 0.0f)
	{
		float t = camShake.time / camShake.duration;
		float strength = camShake.strength * t;

		float time = (float)GetTime();

		camOffset.x = sinf(time * 30.0f + camShake.phase) * strength;
		camOffset.y = cosf(time * 30.0f + camShake.phase) * strength;
	}

	Vector2 baseTarget = gameData.player.physics.transform.pos;

	gameData.camera.target.x = baseTarget.x + camOffset.x;
	gameData.camera.target.y = baseTarget.y + camOffset.y;

#pragma endregion


#pragma region clear background

	ClearBackground({ 75,75,150,255 });

#pragma endregion


#pragma region key bindings

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

	if (IsKeyPressed(KEY_C)) { showCraftUI = !showCraftUI; }

#pragma endregion


#pragma region new player movement

	std::ranlux24_base rng(std::random_device{}());

	static bool creative = false;

	// ── Movement block — replace your existing input section with this ────────────
	// Requires the updated physics.h (updateJump, applyGravity, applyHorizontalMovement).
	// deltaTime should already be defined as GetFrameTime() above this block.

	{
		// ── Horizontal input ─────────────────────────────────────────────────────
		float inputX = 0.f;
		bool  moving = false;

		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
		{
			inputX = -1.f;
			moving = true;
			gameData.player.animations.movingLeft = true;
		}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{
			inputX = 1.f;
			moving = true;
			gameData.player.animations.movingLeft = false;
		}

		// ── Creative mode fly ────────────────────────────────────────────────────
		if (creative)
		{
			if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
				gameData.player.physics.transform.pos.y -= PhysicalEntity::MOVE_SPEED * deltaTime;
			if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
				gameData.player.physics.transform.pos.y += PhysicalEntity::MOVE_SPEED * deltaTime;
			if (GetMouseWheelMove() != 0.f)
				gameData.camera.zoom += GetMouseWheelMove();
		}

		// ── Physics updates (order matters) ──────────────────────────────────────
		// 1. Variable jump + coyote time + jump buffer
		gameData.player.physics.updateJump(
			deltaTime,
			IsKeyDown(KEY_SPACE),    // hold = extend jump
			IsKeyPressed(KEY_SPACE)  // press = queue jump
		);

		// 2. Gravity (with hold/fall multipliers applied inside)
		if (!creative)
			gameData.player.physics.applyGravity();

		// 3. Horizontal movement (ground vs air friction handled inside)
		if (!creative)
			gameData.player.physics.applyHorizontalMovement(deltaTime, inputX);

		// ── Animation ────────────────────────────────────────────────────────────
		bool falling = !gameData.player.physics.downTouch;

		if (falling && gameData.player.physics.velocity.y < 0.f)
			gameData.player.animations.setAnimation(2); // jumping (rising)
		else if (falling)
			gameData.player.animations.setAnimation(3); // falling (descending)
		else if (moving)
			gameData.player.animations.setAnimation(1); // walking
		else
			gameData.player.animations.setAnimation(0); // idle

		gameData.player.animations.update(deltaTime, 0.08, 7);
	}

#pragma endregion


#pragma region old player movement

	//static float CAMERA_SPEED = 10.f;
	//static bool creative = false;

	//{
	//	bool moving = 0;
	//	bool falling = 0;

	//	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	//	{
	//		gameData.player.physics.transform.pos.x -= CAMERA_SPEED * GetFrameTime();
	//		moving = true;
	//		gameData.player.animations.movingLeft = true;
	//	}

	//	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	//	{
	//		gameData.player.physics.transform.pos.x += CAMERA_SPEED * GetFrameTime();
	//		moving = true;
	//		gameData.player.animations.movingLeft = false;
	//	}

	//	if (creative)
	//	{
	//		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) gameData.player.physics.transform.pos.y -= CAMERA_SPEED * GetFrameTime();
	//		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) gameData.player.physics.transform.pos.y += CAMERA_SPEED * GetFrameTime();
	//		if (GetMouseWheelMove() != 0.f) gameData.camera.zoom += GetMouseWheelMove();
	//	}

	//	if (IsKeyPressed(KEY_SPACE))
	//	{
	//		gameData.player.physics.jump(12);
	//	}

	//	if (gameData.player.physics.downTouch)
	//	{
	//		falling = 0;
	//	}
	//	else
	//	{
	//		falling = 1;
	//	}

	//	if (falling)
	//	{
	//		gameData.player.animations.setAnimation(2);
	//	}
	//	else if (moving)
	//	{
	//		gameData.player.animations.setAnimation(1);
	//	}
	//	else
	//	{
	//		gameData.player.animations.setAnimation(0);
	//	}

	//	gameData.player.animations.update(deltaTime, 0.08, 7);
	//}

#pragma endregion


#pragma region entities

	auto updateEntityPhysics = [&](auto& entity, bool applyGravity = true)
		{
			if (applyGravity) { entity.physics.applyGravity(); }
			entity.physics.updateForces(deltaTime);
			entity.physics.resolveConstrains(gameData.gameMap);
			entity.physics.updateFinal();
		};

	updateEntityPhysics(gameData.player, !creative);

	// clamp camera
	{
		float zoom = gameData.camera.zoom;

		float screenWidth = GetScreenWidth();
		float screenHeight = GetScreenHeight();

		// half of visible area (adjusted for zoom)
		float halfViewWidth = (screenWidth * 0.5f) / zoom;
		float halfViewHeight = (screenHeight * 0.5f) / zoom;

		float minX = halfViewWidth;
		float maxX = gameData.gameMap.w - halfViewWidth;
		float minY = halfViewHeight;
		float maxY = gameData.gameMap.h - halfViewHeight;

		// if map is smaller than view (zoomed out a lot), we set the camera to center of the world
		if (maxX < minX)
		{
			gameData.camera.target.x = gameData.gameMap.w * 0.5f;
		}
		else
		{
			gameData.camera.target.x = Clamp(gameData.camera.target.x, minX, maxX);
		}

		if (maxY < minY)
		{
			gameData.camera.target.y = gameData.gameMap.h * 0.5f;
		}
		else
		{
			gameData.camera.target.y = Clamp(gameData.camera.target.y, minY, maxY);
		}
	}

	bool shouldApplyGravity = true;
	float groundDistance = 0;
	bool shouldStepUp = false;

	// update all entities
	for (auto it = gameData.entityHolder.entities.begin(); it != gameData.entityHolder.entities.end();)
	{
		bool shouldKill = false;

		// zombie specific logic
		if (it->second->getEntityType() == EntityType::EntityType_Zombie)
		{
			Vector2 zTotPlayerDirection = gameData.player.getPosition() - it->second->getPosition();

			int nextX = int(it->second->getPosition().x) + 1;
			int prevX = int(it->second->getPosition().x) - 1;

			auto bNext = gameData.gameMap.getBlockSafe(nextX, it->second->getPosition().y);
			auto bPrev = gameData.gameMap.getBlockSafe(prevX, it->second->getPosition().y);

			if (bNext && zTotPlayerDirection.x >= 0 && bNext->type != Block::air && bNext->type != Block::leaves && bNext->type != Block::woodLog)
			{
				shouldStepUp = true;
			}

			if (bPrev && zTotPlayerDirection.x < 0 && bPrev->type != Block::air && bPrev->type != Block::leaves && bPrev->type != Block::woodLog)
			{
				shouldStepUp = true;
			}
		}

		// dropped item specific logic
		if (it->second->getEntityType() == EntityType::EntityType_DroppedItem)
		{
			if (it->second->physics.transform.intersectTransform(gameData.player.physics.transform))
			{
				DroppedItem* d = reinterpret_cast<DroppedItem*>(it->second.get());
				ItemStack itemStack
				{
					d->itemType,
					d->itemCounter
				};
				shouldKill = !gameData.inventory.storeItem(itemStack);
			}
		}

		EntityUpdateData entityUpdateData
		{
			gameData.player.getPosition(),
			rng,
			gameData.entityHolder,
			gameData.inventory,
			it->first,
			groundDistance,
			shouldStepUp
		};


		if (!it->second->update(deltaTime, entityUpdateData) || it->second->life <= 0)
		{
			shouldKill = true;
		}

		if (shouldKill)
		{
			Slime* s = dynamic_cast<Slime*>(it->second.get());

			// check if entity is slime
			if (s != nullptr)
			{
				auto b = gameData.gameMap.getBlockSafe(s->getPosition().x - 0.5f, s->getPosition().y - 0.5f);
				b->type = Block::woodenChest;
			}

			// erase returns next valid iterator
			it = gameData.entityHolder.entities.erase(it);
		}
		else
		{
			// physics
			updateEntityPhysics(*it->second, shouldApplyGravity);
			++it;
		}
	}

#pragma endregion


#pragma region inventory

	for (int i = 0; i < gameData.inventory.items.size(); i++)
	{
		if (gameData.inventory.items[i].itemType != 0 && gameData.inventory.items[i].itemCounter <= 0)
		{
			gameData.inventory.removeItem(i);
		}
	}

#pragma endregion


#pragma region handle input

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (gameData.creativeSelectedBlock < 0) { gameData.creativeSelectedBlock = 0; }
	if (gameData.creativeSelectedBlock >= Block::BLOCKS_COUNT) { gameData.creativeSelectedBlock = Block::BLOCKS_COUNT - 1; }

	// selection
	if (showImgui)
	{
		if (IsKeyPressed(KEY_ONE)) 
		{
			gameData.selectionStart = Vector2{ float(blockX), float(blockY) };
		}
		if (IsKeyPressed(KEY_TWO))
		{
			gameData.selectionEnd = Vector2{ float(blockX), float(blockY) };
		}
		if (IsKeyPressed(KEY_THREE)) 
		{ 
			gameData.copyStructure.pasteIntoMap(gameData.gameMap, Vector2{ float(blockX), float(blockY) }); 
		}

		if (gameData.selectionStart.x > gameData.selectionEnd.x)
		{
			std::swap(gameData.selectionStart.x, gameData.selectionEnd.x);
		}
		if (gameData.selectionStart.y > gameData.selectionEnd.y)
		{
			std::swap(gameData.selectionStart.y, gameData.selectionEnd.y);
		}

	}

	if (!showImgui && !showCraftUI)
	{
		if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_MIDDLE))
		{
			auto b = gameData.gameMap.getBlockSafe(blockX, blockY);

			if (b)
			{
				gameData.creativeSelectedBlock = b->type;
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			// hit entities (enemies)
			for (auto& e : gameData.entityHolder.entities)
			{
				DroppedItem* droppedItem = dynamic_cast<DroppedItem*>(e.second.get());

				if (
					droppedItem == nullptr && 
					e.second->physics.transform.intersectPoint(worldPos) && 
					gameData.player.timeAfterAttack <= 0
				)
				{
					// Hitting an enemy
					int dmg = calcMeleeDamage(gameData.player.heldItem);

					// get reset time
					float attackResetTime = getResetTime(gameData.player.heldItem);

					// reset attack time
					if (attackResetTime != 0)
						gameData.player.timeAfterAttack = attackResetTime;

					// reduce health from enemy
					e.second->hit(dmg);

					// camera shake
					triggerCameraShake(0.2f, 0.08f);
				}
			}

			// spawn block
			float magnitude = Vector2Distance(gameData.player.getPosition(), worldPos);
			if (magnitude <= 5 || creative)
			{
				auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
				if (b)
				{
					if (b->type && gameData.player.timeAfterMine <= 0)
					{
						// calculate damage done to block
						int dmg = calcBlockDamage(*b, gameData.player.heldItem);
						b->hp -= dmg;

						if (dmg > 0)
						{
							// add block shake here
							triggerShake(blockX, blockY);
						}

						// get reset time, 0.7 default for bare hands, 0 for non-tool items
						float toolResetTime = getResetTime(gameData.player.heldItem);

						if (toolResetTime != 0)
						{
							gameData.player.timeAfterMine = toolResetTime;
						}

						if (b->hp <= 0)
						{
							spawnDroppedItem({ (float)blockX + 0.5f, (float)blockY + 0.5f }, b->type);
							*b = {};
						}
					}
				}
			}
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			float magnitude = Vector2Distance(gameData.player.getPosition(), worldPos);
			if (magnitude <= 5 || creative)
			{
				// place block only if its withing reach, 
				// no existing blocks and 
				// adjecent block exists
				auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
				if (b && b->type == Block::air && gameData.gameMap.isAdjacentBlock(blockX, blockY))
				{
					for (auto& i : gameData.inventory.items)
					{
						// check inventory to see if we have same type of item and have more than 0
						if (gameData.creativeSelectedBlock == i.itemType && i.itemCounter > 0)
						{
							b->type = i.itemType;
							i.itemCounter -= 1;
						}
					}
				}
			}
		}
	}

#pragma endregion


#pragma region craft ui

	// craft ui

	if (showCraftUI)
	{
		ImGui::Begin("Craft");

		// input slots
		for (int i = 0; i < gameData.maxCraftSlots; i++)
		{
			ImGui::PushID(i);

			int item = gameData.craftSlots[i];

			auto atlas = getTextureAtlas(item, 0, 32, 32);
			ImTextureID tex;

			if (item < Block::BLOCKS_COUNT)
			{
				atlas = getUVForTexture(assetManager.textures, atlas);
				tex = (ImTextureID)(intptr_t)assetManager.textures.id;
			}
			else
			{
				atlas = getUVForTexture(assetManager.items, atlas);
				tex = (ImTextureID)(intptr_t)assetManager.items.id;
			}

			// draw image button
			if (ImGui::ImageButton(
				tex,
				{ 40,40 },
				{ atlas.x,atlas.y },
				{ atlas.x + atlas.width,atlas.y + atlas.height }
			))
			{
				// assign block back to inventory
				ItemStack item{ gameData.craftSlots[i], 1 };
				gameData.inventory.storeItem(item);

				// clear craft slot
				gameData.craftSlots[i] = 0;
			}

			ImGui::PopID();
			ImGui::SameLine();

			ImGui::Text(i != gameData.maxCraftSlots - 1 ? "+" : "=");

			ImGui::SameLine();
		}

		int result = 0;

		// preview in output slot
		if (gameData.inventory.canCraft(gameData.craftSlots))
		{
			result = gameData.inventory.craft(gameData.craftSlots);
		}

		// spawn item if click on output slot
		auto atlas = getTextureAtlas(result, 0, 32, 32);
		ImTextureID tex;

		if (result < Block::BLOCKS_COUNT)
		{
			atlas = getUVForTexture(assetManager.textures, atlas);
			tex = (ImTextureID)(intptr_t)assetManager.textures.id;
		}
		else
		{
			atlas = getUVForTexture(assetManager.items, atlas);
			tex = (ImTextureID)(intptr_t)assetManager.items.id;
		}

		if (ImGui::ImageButton(
			tex,
			{ 40, 40 },
			{ atlas.x, atlas.y },
			{ atlas.x + atlas.width, atlas.y + atlas.height }
		))
		{
			if (gameData.inventory.canCraft(gameData.craftSlots))
			{
				int item = gameData.inventory.craft(gameData.craftSlots);

				// spawn item close to player (with offset)
				// if too close, player will immediately pick up
				spawnDroppedItem(
					{ gameData.player.getPosition().x + 2.5f, gameData.player.getPosition().y + 2.5f },
					item
				);

				// clear slots after crafting
				gameData.craftSlots[0] = 0;
				gameData.craftSlots[1] = 0;
			}
		}

		ImGui::End();
	}

#pragma endregion


#pragma region draw world

	// background
	{
		int backgroundType = DrawBackground::forest;

		if (gameData.player.getPosition().x > gameData.gameMap.desertStart &&
			gameData.player.getPosition().x < gameData.gameMap.desertEnd)
		{
			backgroundType = DrawBackground::desert;
		}

		if (gameData.player.getPosition().y > 130)
		{
			backgroundType = DrawBackground::cave;
		}

		gameData.background.setBackground(backgroundType);

		gameData.background.draw(
			deltaTime,
			assetManager,
			gameData.camera,
			{ (float)gameData.gameMap.w, (float)gameData.gameMap.h }
		);
	}

	BeginMode2D(gameData.camera);

	Vector2 topLeftView = GetScreenToWorld2D({ 0,0 }, gameData.camera);
	Vector2 bottomRightView = GetScreenToWorld2D({ (float)GetScreenWidth(), (float)GetScreenHeight() }, gameData.camera);

	int startXView = (int)floorf(topLeftView.x - 1);
	int endXView = (int)floorf(bottomRightView.x + 1);
	int startYView = (int)floorf(topLeftView.y - 1);
	int endYView = (int)floorf(bottomRightView.y + 1);

	startXView = Clamp((float)startXView, 0.f, (float)gameData.gameMap.w - 1);
	endXView = Clamp((float)endXView, 0.f, (float)gameData.gameMap.w - 1);

	startYView = Clamp((float)startYView, 0.f, (float)gameData.gameMap.h - 1);
	endYView = Clamp((float)endYView, 0.f, (float)gameData.gameMap.h - 1);

	for (int y = startYView; y <= endYView; y++)
	{
		for (int x = startXView; x <= endXView; x++)
		{
			float size = 1;
			float posX = x * size;
			float posY = y * size;

			int atlasX = 0;

			auto& bb = gameData.backgroundMap.getBlockUnsafe(x, y);

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

			auto& b = gameData.gameMap.getBlockUnsafe(x, y);


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
					WHITE    //tint
				);
			}
		}
	}

#pragma endregion


#pragma region draw frame and selected block

	//draw selected block
	DrawTexturePro(
		assetManager.frame,
		{ 0,0,(float)assetManager.frame.width,(float)assetManager.frame.height },
		{ (float)blockX, (float)blockY, 1, 1 },
		{},
		0.f,
		WHITE
	);

	DrawTexturePro(
		assetManager.textures,
		getTextureAtlas(gameData.creativeSelectedBlock, 0, 32, 32),
		{ (float)blockX, (float)blockY, 1, 1 },
		{},
		0.f,
		{ 255,255,255,127 }
	);

#pragma endregion


#pragma region draw entities

	for (auto& e : gameData.entityHolder.entities)
	{
		e.second->render(assetManager);
	}

#pragma endregion


#pragma region draw player

	gameData.player.render(assetManager);

#pragma endregion


#pragma region draw inventory

	if (!showImgui)
	{
		ImGui::Begin("Inventory");

		for (int i = 0; i < gameData.inventory.slots; i++)
		{
			auto atlas = getTextureAtlas(gameData.inventory.items[i].itemType, 0, 32, 32);
			ImTextureID tex;

			if (gameData.inventory.items[i].itemType < Block::BLOCKS_COUNT)
			{
				atlas = getUVForTexture(assetManager.textures, atlas);
				tex = (ImTextureID)(intptr_t)assetManager.textures.id;
			}
			else
			{
				atlas = getUVForTexture(assetManager.items, atlas);
				tex = (ImTextureID)(intptr_t)assetManager.items.id;
			}

			ImGui::PushID(i);

			// draw image button
			if (ImGui::ImageButton(
				tex,
				{ 35,35 },
				{ atlas.x,atlas.y },
				{ atlas.x + atlas.width,atlas.y + atlas.height }
			))
			{
				gameData.creativeSelectedBlock = gameData.inventory.items[i].itemType;
				gameData.player.heldItem = gameData.creativeSelectedBlock;

				if (showCraftUI)
				{
					for (int j = 0; j < gameData.maxCraftSlots; j++)
					{
						if (gameData.craftSlots[j] == 0)
						{
							gameData.craftSlots[j] = gameData.creativeSelectedBlock;
							gameData.inventory.items[i].itemCounter -= 1;
							break;
						}
					}
				}
			}

			// get button position
			ImVec2 min = ImGui::GetItemRectMin();
			ImVec2 max = ImGui::GetItemRectMax();

			// draw text on top (bottom-right corner)
			if (gameData.inventory.items[i].itemType != 0)
			{
				std::string count = std::to_string(gameData.inventory.items[i].itemCounter);

				ImDrawList* drawList = ImGui::GetWindowDrawList();

				ImVec2 textSize = ImGui::CalcTextSize(count.c_str());
				ImVec2 textPos = ImVec2(
					max.x - textSize.x - 2,
					max.y - textSize.y - 2
				);

				drawList->AddText(textPos, IM_COL32(255, 255, 255, 127), count.c_str());
			}

			ImGui::PopID();
			ImGui::SameLine();
		}

		ImGui::End();
	}

#pragma endregion


#pragma region show structure selection

	// show structure selection
	if (showImgui)
	{
		Rectangle rect;
		rect.x = gameData.selectionStart.x;
		rect.y = gameData.selectionStart.y;
		rect.width = gameData.selectionEnd.x - gameData.selectionStart.x;
		rect.height = gameData.selectionEnd.y - gameData.selectionStart.y;

		rect.width++;
		rect.height++;

		DrawRectangleLinesEx(
			rect,
			0.1,
			{ 20,101,250,145 }
		);
	}

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


#pragma region ImGui Madness

	if (showImgui)
	{
		ImGui::Begin("Game Control");

		ImGui::SliderFloat("Camera Zoom: ", &gameData.camera.zoom, 10, 150);
		//ImGui::SliderFloat("Camera Speed: ", &CAMERA_SPEED, 5, 100);

		ImGui::Checkbox("Creative", &creative);

		if (ImGui::Button("Spawn slime"))
		{
			spawnSlime({ 18,60 });
		}

		if (ImGui::Button("Hurt a slime"))
		{
			for (auto& e : gameData.entityHolder.entities)
			{
				if (e.second->getEntityType() == EntityType::EntityType_Slime)
				{
					e.second->life -= 3;
					break;
				}
			}
		}

		ImGui::InputText(("Texture Pack"), gameData.texturePackName, sizeof(gameData.texturePackName));
		if (ImGui::Button("Load Texture"))
		{
			assetManager.loadTexturePack(gameData.texturePackName);
		}

		ImGui::Separator();

		if (ImGui::Button("Copy"))
		{
			gameData.copyStructure.copyFromMap(gameData.gameMap, gameData.selectionStart, gameData.selectionEnd);
		}

		ImGui::InputText("File name", gameData.saveName, sizeof(gameData.saveName));

		if (ImGui::Button("Save to file"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			saveBlockDataToFile(
				gameData.copyStructure.mapData,
				gameData.copyStructure.w,
				gameData.copyStructure.h,
				path.c_str()
			);
		}

		if (ImGui::Button("Load from file"))
		{
			std::string path = RESOURCES_PATH "structures/";
			path += gameData.saveName;
			path += ".bin";

			bool d = loadBlockDataFromFile(
				gameData.copyStructure.mapData,
				gameData.copyStructure.w,
				gameData.copyStructure.h,
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
				gameData.creativeSelectedBlock = i;
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

	DrawFPS(10, 10);

#pragma endregion

	return true;
}

#pragma region close game logic

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}

#pragma endregion