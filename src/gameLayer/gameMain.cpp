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
#include <worldGenerator.h>
#include <helper.h>
#include <randomStuff.h>

#include <structure.h>
#include <saveMap.h>
#include <physics.h>

#include <entityHolder.h>
#include <entities/slime.h>
#include <entities/droppedItem.h>
#include <player.h>


#pragma region global variables

struct GameData
{
	GameMap backgroundMap = {}; 
	GameMap gameMap = {};
	Camera2D camera = {};

	int creativeSelectedBlock = Block::air;

	Vector2 selectionStart = {};
	Vector2 selectionEnd = {};

	Structure copyStructure;

	Player player;
	EntityHolder entityHolder;
	
	//std::unordered_set<int> randomisedItems = {};

	char saveName[100] = {};

} gameData;

AssetManager assetManager;

bool showImgui = false;

#pragma endregion


#pragma region helper imgui functions

void spawnSlime(Vector2 position)
{
	Slime slime;
	slime.teleport(position);

	auto id = gameData.entityHolder.idHolder.getEntityIdAndIncreament();
	gameData.entityHolder.entities[id] = std::make_unique<Slime>(slime);
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
	//gameData.randomisedItems = generateRandomItemArray(100);
	//for (auto& i : gameData.randomisedItems)
	//{
	//	printf("%d\n", i);
	//}

	assetManager.loadAll();
	int w = 900, h = 500;

	gameData.backgroundMap.create(w, h);
	generateWorld(gameData.gameMap);

	gameData.camera.target = { 20, 120 };
	gameData.camera.rotation = 0.f;
	gameData.camera.zoom = 50.f;

	gameData.player.teleport({ 20, 60 });
	gameData.player.physics.transform.w = 0.9f;
	gameData.player.physics.transform.h = 1.8f;

	spawnSlime({ 18,60 });

	return true;
}

bool updateGame()
{

#pragma region delta time

	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5.f) deltaTime = 1 / 5.f;

#pragma endregion


#pragma region clear background

	ClearBackground({ 75,75,150,255 });

#pragma endregion


#pragma region f10

	if (IsKeyPressed(KEY_F10)) { showImgui = !showImgui; }

#pragma endregion


#pragma region camera movement

	gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };

	static float CAMERA_SPEED = 20.f;
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) gameData.player.physics.transform.pos.x -= CAMERA_SPEED * GetFrameTime();
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) gameData.player.physics.transform.pos.x += CAMERA_SPEED * GetFrameTime();
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) gameData.player.physics.transform.pos.y -= CAMERA_SPEED * GetFrameTime();
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) gameData.player.physics.transform.pos.y += CAMERA_SPEED * GetFrameTime();

	if (GetMouseWheelMove() != 0.f) gameData.camera.zoom += GetMouseWheelMove();
	if (IsKeyPressed(KEY_SPACE)) gameData.player.physics.jump(10);

#pragma endregion


#pragma region entities

	auto updateEntityPhysics = [&](auto& entity, bool applyGravity = true)
		{
			if (applyGravity) { entity.physics.applyGravity(); }
			entity.physics.updateForces(deltaTime);
			entity.physics.resolveConstrains(gameData.gameMap);
			entity.physics.updateFinal();
		};

	updateEntityPhysics(gameData.player);
	gameData.camera.target = gameData.player.getPosition();

	std::ranlux24_base rng(std::random_device{}());

	// update all entities
	for (auto it = gameData.entityHolder.entities.begin(); it != gameData.entityHolder.entities.end();)
	{
		EntityUpdateData entityUpdateData
		{
			gameData.player.getPosition(),
			rng,
			gameData.entityHolder,
			it->first
		};

		bool shouldKill = false;

		if (!it->second->update(deltaTime, entityUpdateData) || it->second->life <= 0)
		{
			shouldKill = true;
		}

		if (shouldKill)
		{
			// erase returns next valid iterator
			it = gameData.entityHolder.entities.erase(it);
		}
		else
		{
			// physics
			updateEntityPhysics(*it->second);
			++it;
		}
	}

#pragma endregion


#pragma region mouse input

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (gameData.creativeSelectedBlock < 0) { gameData.creativeSelectedBlock = 0; }
	if (gameData.creativeSelectedBlock >= Block::BLOCKS_COUNT) { gameData.creativeSelectedBlock = Block::BLOCKS_COUNT - 1; }

	//// calculate dist from player to mouse pos (NOTE: currently player is camera target)
	//Vector2 dist = {
	//gameData.camera.target.x - (blockX + 0.5),
	//gameData.camera.target.y - (blockY + 0.5)
	//};
	//float magnitude = std::sqrt(((dist.x * dist.x) + (dist.y * dist.y)));

	//printf("dist: %f, %f\nmagnitude: %f\n", dist.x, dist.y, magnitude);

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

	if (!showImgui)
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
			std::ranlux24_base rng;

			auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
			if (b)
			{
				if (b->type)
				{
					float xOffset = getRandomFloat(rng, -1, 1);
					float yOffset = getRandomFloat(rng, -1, 1);
					spawnDroppedItem({ (float)blockX + xOffset, (float)blockY + yOffset }, b->type);
				}
				*b = {};
			}
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			//if (magnitude <= 5)
			//{
				//// place block only if its withing reach
			//}

			auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
			if (b)
			{
				b->type = gameData.creativeSelectedBlock;

#pragma region random item

				//b->type = editorState.selectedTile;
				//if (gameData.randomisedItems.empty())
				//{
				//	gameData.randomisedItems = generateRandomItemArray(100);
				//}
				//else
				//{
				//	auto i = gameData.randomisedItems.begin();
				//	int val = *i;
				//	gameData.randomisedItems.erase(i);
				//	//std::cout << "Popped: " << val << std::endl;

				//	//common 0-39 40
				//	//uncommon 40-69 30
				//	//rare 70-88 19
				//	//epic 89-97 9
				//	//legendary 98-99 2

				//	if (val < 40)
				//		b->type = Block::dirt;

				//	else if (val >= 40 && val < 70)
				//		b->type = Block::bricks;

				//	else if (val >= 70 && val < 89)
				//		b->type = Block::silverBlockWall;

				//	else if (val >= 89 && val < 98)
				//		b->type = Block::goldBlock;

				//	else if (val == 98 || val == 99)
				//	{
				//		b->type = Block::rubyBlock;
				//		printf("congrats! you found legendary loot! resetting...");
				//		gameData.randomisedItems = generateRandomItemArray(100);
				//	}

				//	else
				//	{
				//		printf("\n\nDANGER: value out of bounds, not allowed\n\n");
				//		b->type = Block::air;
				//	}
				//}

#pragma endregion

			}
		}
	}

#pragma endregion


#pragma region draw world

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
#pragma region draw tree
				//Block* upperBlock = gameData.gameMap.getBlockSafe(x, y - 1);
				//Block* belowBlock = gameData.gameMap.getBlockSafe(x, y + 1);

				//if (!upperBlock || !belowBlock) continue;

				//if (b.type == Block::woodLog)
				//{
				//	if (belowBlock->type == Block::grassBlock)
				//	{
				//		DrawTexturePro(
				//			assetManager.treeTextures,
				//			getTextureAtlas(7, 3, 32, 32), //source (in sprite)
				//			{ posX,posY,size,size }, //dest
				//			{ 0,0 }, //origin (top-left)
				//			0.f,     //rotation
				//			WHITE    //tint
				//		);
				//	}

				//	if (upperBlock->type == Block::leaves && b.type == Block::woodLog)
				//	{
				//		DrawTexturePro(
				//			assetManager.treeTextures,
				//			getTextureAtlas(5, 3, 32, 32), //source (in sprite)
				//			{ posX,posY,size,size }, //dest
				//			{ 0,0 }, //origin (top-left)
				//			0.f,     //rotation
				//			WHITE    //tint
				//		);
				//		continue;
				//	}
				//}

				//Color color = WHITE;

				//if (b.type == Block::leaves)
				//{
				//	if (x % 2 == 0)
				//		color = DARKBLUE;
				//	else
				//		color = GREEN;

				//	if (y % 2 == 0)
				//		color.a = 127;
				//}
#pragma endregion

				atlasX = b.type;

				if (b.variation == -1)
				{
					b.variation = rand() % 4;
				}

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(atlasX, b.variation, 32, 32), //source (in sprite)
					{ posX,posY,size,size }, //dest
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


#pragma region draw slime

	for (auto& e : gameData.entityHolder.entities)
	{
		e.second->render(assetManager);
	}

#pragma endregion


#pragma region draw player

	Transform2D playerSprite = gameData.player.physics.transform;
	playerSprite.w = 1;
	playerSprite.h = 2;

	playerSprite.pos.y -= (playerSprite.h - gameData.player.physics.transform.h) / 2;

	gameData.player.render(assetManager);

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
		ImGui::SliderFloat("Camera Speed: ", &CAMERA_SPEED, 5, 100);

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