#include "gameplay.h"

#include <imgui.h>

#include <ui.h>
#include <audio.h>
#include <settings.h>
#include <helper.h>
#include <saveMap.h>
#include <worldGenerator.h>

#include <shake.h>
#include <entities/droppedItem.h>
#include <entities/slime.h>
#include <entities/desetSlime.h>
//#include <entities/evilEye.h>
#include <entities/zombie.h>

#include <itemData.h>

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

bool Gameplay::init()
{
	craftSlots.resize(maxCraftSlots);

	int w = 900, h = 500;

	backgroundMap.create(w, h);
	generateWorld(gameMap);

	camera.target = { 20, 120 };
	camera.rotation = 0.f;
	camera.zoom = CAMERA_ZOOM;

	player.teleport({ 20, 60 });
	player.physics.transform.w = 0.9f;
	player.physics.transform.h = 1.8f;

	// Light mask render texture
	lightMask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	camFollow.init(1.5f, .74f, 1.f, player.getPosition());

	spawnZombie({ 25,60 });

	//spawnDroppedItem({ 25,60 }, 6001);

	return true;
}

bool Gameplay::update(AssetManager& assetManager)
{

#pragma region fixed updates

	Audio::update();
	updateSettings();

#pragma endregion


#pragma region delta time

	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5.f) deltaTime = 1 / 5.f;

#pragma endregion


#pragma region reset attck & animation times

	if (player.timeAfterMine > 0)
		player.timeAfterMine -= deltaTime;

	if (player.timeAfterAttack > 0)
		player.timeAfterAttack -= deltaTime;

	if (player.timeAfterAttackAnimation > 0.0f)
	{
		player.timeAfterAttackAnimation -= deltaTime;

		if (player.timeAfterAttackAnimation < 0.0f)
			player.timeAfterAttackAnimation = 0.0f;
	}

#pragma endregion


#pragma region updates (deltatime dependent)

	updateShake(deltaTime);
	updateCameraShake(deltaTime);
	updateParticles(particles, deltaTime);

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

	ClearBackground({ 75,75,150,255 });

#pragma endregion


#pragma region key bindings

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

	if (IsKeyPressed(KEY_C)) { showCraftUI = !showCraftUI; }

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) { saveWorld(gameMap, entityHolder, player); }

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) { loadWorld(gameMap, entityHolder, player); }

#pragma endregion


#pragma region new player movement

	std::ranlux24_base rng(std::random_device{}());

	static bool creative = false;

	// ← snapshot HERE, before any physics or input runs
	bool wasTouchingGround = player.physics.downTouch;
	float landingVelocity = player.physics.velocity.y;

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
			player.animations.movingLeft = true;
		}
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
		{
			inputX = 1.f;
			moving = true;
			player.animations.movingLeft = false;
		}

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

		// ── Physics updates (order matters) ──────────────────────────────────────
		// 1. Variable jump + coyote time + jump buffer
		player.physics.updateJump(
			deltaTime,
			IsKeyDown(KEY_SPACE),    // hold = extend jump
			IsKeyPressed(KEY_SPACE)  // press = queue jump
		);

		// 2. Gravity (with hold/fall multipliers applied inside)
		if (!creative)
			player.physics.applyGravity();

		// 3. Horizontal movement (ground vs air friction handled inside)
		//if (!creative)
		player.physics.applyHorizontalMovement(deltaTime, inputX);

		// ── Animation ────────────────────────────────────────────────────────────
		bool falling = !player.physics.downTouch;

		if (falling && player.physics.velocity.y < 0.f)
			player.animations.setAnimation(2); // jumping (rising)
		else if (falling)
			player.animations.setAnimation(3); // falling (descending)
		else if (moving)
			player.animations.setAnimation(1); // walking
		else
			player.animations.setAnimation(0); // idle

		player.animations.update(deltaTime, 0.08, 7);
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
	//		player.physics.transform.pos.x -= CAMERA_SPEED * GetFrameTime();
	//		moving = true;
	//		player.animations.movingLeft = true;
	//	}

	//	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	//	{
	//		player.physics.transform.pos.x += CAMERA_SPEED * GetFrameTime();
	//		moving = true;
	//		player.animations.movingLeft = false;
	//	}

	//	if (creative)
	//	{
	//		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player.physics.transform.pos.y -= CAMERA_SPEED * GetFrameTime();
	//		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.physics.transform.pos.y += CAMERA_SPEED * GetFrameTime();
	//		if (GetMouseWheelMove() != 0.f) camera.zoom += GetMouseWheelMove();
	//	}

	//	if (IsKeyPressed(KEY_SPACE))
	//	{
	//		player.physics.jump(12);
	//	}

	//	if (player.physics.downTouch)
	//	{
	//		falling = 0;
	//	}
	//	else
	//	{
	//		falling = 1;
	//	}

	//	if (falling)
	//	{
	//		player.animations.setAnimation(2);
	//	}
	//	else if (moving)
	//	{
	//		player.animations.setAnimation(1);
	//	}
	//	else
	//	{
	//		player.animations.setAnimation(0);
	//	}

	//	player.animations.update(deltaTime, 0.08, 7);
	//}

#pragma endregion


#pragma region entities

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
			it->first,
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


#pragma region inventory - needs refactoring

	for (int i = 0; i < inventory.items.size(); i++)
	{
		if (inventory.items[i].itemType != 0 && inventory.items[i].itemCounter <= 0)
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
		if (!insideInventoryMenu)
			if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_MIDDLE))
			{
				auto b = gameMap.getBlockSafe(blockX, blockY);

				if (b)
				{
					creativeSelectedBlock = b->type;
				}
			}

		if (!insideInventoryMenu)
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				// hit entities (enemies)
				for (auto& e : entityHolder.entities)
				{
					DroppedItem* droppedItem = dynamic_cast<DroppedItem*>(e.second.get());
					float magnitude = Vector2Distance(player.physics.transform.getCenter(), worldPos);

					if (
						droppedItem == nullptr &&
						player.physics.downTouch &&
						e.second->physics.transform.intersectPoint(worldPos) &&
						player.timeAfterAttack <= 0 &&
						(isInRange(player.heldItem, magnitude) || creative)
						)
					{
						// play attack animation
						player.timeAfterAttackAnimation = player.maxAttackTimeAnimation;

						// Hitting an enemy
						int dmg = calcMeleeDamage(player.heldItem);

						// get reset time
						float attackResetTime = getResetTime(player.heldItem);

						// reset attack time
						if (attackResetTime != 0)
							player.timeAfterAttack = attackResetTime;

						// reduce health from enemy
						e.second->hit(dmg);

						// camera shake
						triggerCameraShake(0.2f, 0.08f);
					}
				}

				// spawn block
				float magnitude = Vector2Distance(player.physics.transform.getCenter(), worldPos);
				auto b = gameMap.getBlockSafe(blockX, blockY);
				if (
					b &&
					b->type &&
					player.physics.downTouch &&
					player.timeAfterMine <= 0 &&
					(isInRange(player.heldItem, magnitude) || creative)
					)
				{
					// play attack animation
					player.timeAfterAttackAnimation = player.maxAttackTimeAnimation;

					// particle effect
					auto newParticles = spawnParticles({ (float)blockX, (float)blockY }, rng, b->type, 10);
					particles.insert(particles.end(), newParticles.begin(), newParticles.end());

					// calculate damage done to block
					int dmg = calcBlockDamage(*b, player.heldItem);
					b->hp -= dmg;

					if (dmg > 0)
					{
						// add block shake here
						triggerShake(blockX, blockY);
					}

					// get reset time, 0.7 default for bare hands, 0 for non-tool items
					float toolResetTime = getResetTime(player.heldItem);

					if (toolResetTime != 0)
					{
						player.timeAfterMine = toolResetTime;
					}

					if (b->hp <= 0)
					{
						spawnDroppedItem({ (float)blockX + 0.5f, (float)blockY + 0.5f }, b->type);
						*b = {};
					}
				}
			}

		if (!insideInventoryMenu)
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
						for (auto& i : inventory.items)
						{
							// check inventory to see if we have same type of item and have more than 0
							if (creativeSelectedBlock == i.itemType && i.itemCounter > 0)
							{
								b->type = i.itemType;
								i.itemCounter -= 1;
							}
						}
					}
				}
			}

		if (IsKeyPressed(KEY_TAB))
		{
			insideInventory = !insideInventory;
		}
	}

#pragma endregion


#pragma region craft ui - needs refactoring

	// craft ui

	if (showCraftUI)
	{
		ImGui::Begin("Craft");

		// input slots
		for (int i = 0; i < maxCraftSlots; i++)
		{
			ImGui::PushID(i);

			int item = craftSlots[i];

			auto atlas = getTextureCoordinatesForItemType(item);
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
				ItemStack item{ craftSlots[i], 1 };
				inventory.storeItem(item);

				// clear craft slot
				craftSlots[i] = 0;
			}

			ImGui::PopID();
			ImGui::SameLine();

			ImGui::Text(i != maxCraftSlots - 1 ? "+" : "=");

			ImGui::SameLine();
		}

		int result = 0;

		// preview in output slot
		if (inventory.canCraft(craftSlots))
		{
			result = inventory.craft(craftSlots);
		}

		// spawn item if click on output slot
		auto atlas = getTextureCoordinatesForItemType(result);
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
			if (inventory.canCraft(craftSlots))
			{
				int item = inventory.craft(craftSlots);

				// spawn item close to player, so its immediately picked up and added to inventory
				spawnDroppedItem(
					{ player.getPosition().x, player.getPosition().y },
					item
				);

				// clear slots after crafting
				craftSlots[0] = 0;
				craftSlots[1] = 0;
			}
		}

		ImGui::End();
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
	if (!insideInventoryMenu)
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
	}

#pragma endregion


#pragma region render player

	player.render(assetManager);

#pragma endregion


#pragma region render inventory

	//if (!showImgui)
	//{
	//	ImGui::Begin("Inventory");

	//	for (int i = 0; i < inventory.slots; i++)
	//	{
	//		auto atlas = getTextureCoordinatesForItemType(inventory.items[i].itemType);
	//		ImTextureID tex;

	//		if (inventory.items[i].itemType < Block::BLOCKS_COUNT)
	//		{
	//			atlas = getUVForTexture(assetManager.textures, atlas);
	//			tex = (ImTextureID)(intptr_t)assetManager.textures.id;
	//		}
	//		else
	//		{
	//			atlas = getUVForTexture(assetManager.items, atlas);
	//			tex = (ImTextureID)(intptr_t)assetManager.items.id;
	//		}

	//		ImGui::PushID(i);

	//		// draw image button
	//		if (ImGui::ImageButton(
	//			tex,
	//			{ 35,35 },
	//			{ atlas.x,atlas.y },
	//			{ atlas.x + atlas.width,atlas.y + atlas.height }
	//		))
	//		{
	//			creativeSelectedBlock = inventory.items[i].itemType;
	//			player.heldItem = creativeSelectedBlock;

	//			if (showCraftUI)
	//			{
	//				for (int j = 0; j < maxCraftSlots; j++)
	//				{
	//					if (craftSlots[j] == 0)
	//					{
	//						craftSlots[j] = creativeSelectedBlock;
	//						inventory.items[i].itemCounter -= 1;
	//						break;
	//					}
	//				}
	//			}
	//		}

	//		// get button position
	//		ImVec2 min = ImGui::GetItemRectMin();
	//		ImVec2 max = ImGui::GetItemRectMax();

	//		// draw text on top (bottom-right corner)
	//		if (inventory.items[i].itemType != 0)
	//		{
	//			std::string count = std::to_string(inventory.items[i].itemCounter);

	//			ImDrawList* drawList = ImGui::GetWindowDrawList();

	//			ImVec2 textSize = ImGui::CalcTextSize(count.c_str());
	//			ImVec2 textPos = ImVec2(
	//				max.x - textSize.x - 2,
	//				max.y - textSize.y - 2
	//			);

	//			drawList->AddText(textPos, IM_COL32(255, 255, 255, 127), count.c_str());
	//		}

	//		ImGui::PopID();
	//		ImGui::SameLine();
	//	}

	//	ImGui::End();
	//}

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

#pragma	region ui

	float w = GetScreenWidth();
	float h = GetScreenHeight();

	Rectangle heartRectangle;
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

		Rectangle oneCellRectangle;
		oneCellRectangle.height = inventoryRectangle.height / 3;
		oneCellRectangle.width = oneCellRectangle.height;
		oneCellRectangle.x = inventoryRectangle.x;
		oneCellRectangle.y = inventoryRectangle.y;

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Rectangle r = oneCellRectangle;
				r.x += i * oneCellRectangle.width;
				r.y += j * oneCellRectangle.height;

				r = shrinkRectanglePercentage(r, .1f, .1f);

				if (CheckCollisionPointRec(GetMousePosition(), r))
				{
					DrawTexturePro(
						assetManager.frame,
						getTextureAtlas(0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height),
						r,
						{ 0,0 },
						0.f,
						{ 220,250,220,250 }
					);
				}
				else
				{
					DrawTexturePro(
						assetManager.frame,
						getTextureAtlas(0, 0, (float)assetManager.frame.width, (float)assetManager.frame.height),
						r,
						{ 0,0 },
						0.f,
						{ 180,180,200,240 }
					);
				}
			}
		}
	}

#pragma endregion


#pragma region lighting

	// Apply cave lighting ONLY when underground
	if (player.getPosition().y > 130)
	{
		// Light mask render texture
		lightMask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

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
			{ 0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight() }, // negative H = flip Y
			{ 0, 0 },
			WHITE
		);
		EndBlendMode();
	}

#pragma endregion


#pragma region ImGui Madness

	if (showImgui)
	{
		ImGui::Begin("Game Control");

		ImGui::SliderFloat("Camera Zoom: ", &camera.zoom, 10, 150);
		//ImGui::SliderFloat("Camera Speed: ", &CAMERA_SPEED, 5, 100);

		ImGui::Checkbox("Creative", &creative);

		if (ImGui::Button("Spawn slime"))
		{
			spawnSlime({ 18,60 });
		}

		if (ImGui::Button("Hurt a slime"))
		{
			for (auto& e : entityHolder.entities)
			{
				if (e.second->getEntityType() == EntityType::EntityType_Slime)
				{
					e.second->life -= 3;
					break;
				}
			}
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

	DrawFPS(10, 10);

#pragma endregion

	return true;
}