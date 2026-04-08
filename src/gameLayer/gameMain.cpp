#include <iostream>
#include <fstream>
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

struct GameData
{
	GameMap gameMap = {};
	Camera2D camera = {};

} gameData;

AssetManager assetManager;



bool initGame()
{
	assetManager.loadAll();

	generateWorld(gameData.gameMap);

	//gameData.gameMap.getBlockUnsafe(5, 7).type = Block::leaves;
	//gameData.gameMap.getBlockUnsafe(5, 8).type = Block::leaves;
	//gameData.gameMap.getBlockUnsafe(5, 9).type = Block::leaves;

	//gameData.gameMap.getBlockUnsafe(4, 8).type = Block::leaves;
	//gameData.gameMap.getBlockUnsafe(4, 9).type = Block::leaves;
	//gameData.gameMap.getBlockUnsafe(4, 10).type = Block::leaves;

	//gameData.gameMap.getBlockUnsafe(6, 8).type = Block::leaves;
	//gameData.gameMap.getBlockUnsafe(6, 9).type = Block::leaves;
	//gameData.gameMap.getBlockUnsafe(6, 10).type = Block::leaves;


	//gameData.gameMap.getBlockUnsafe(5, 10).type = Block::woodLog;
	//gameData.gameMap.getBlockUnsafe(5, 11).type = Block::woodLog;
	//gameData.gameMap.getBlockUnsafe(5, 12).type = Block::woodLog;

	//gameData.gameMap.getBlockUnsafe(4, 13).type = Block::grassBlock;
	//gameData.gameMap.getBlockUnsafe(5, 13).type = Block::grassBlock;
	//gameData.gameMap.getBlockUnsafe(6, 13).type = Block::grassBlock;

	gameData.camera.target = { 0,0 };
	gameData.camera.rotation = 0.f;
	gameData.camera.zoom = 10.f;

	return true;
}

bool updateGame()
{
	float deltaTime = GetFrameTime();
	if (deltaTime > 1.f / 5.f) deltaTime = 1 / 5.f;

	gameData.camera.offset = { GetScreenWidth() / 2.f, GetScreenHeight() / 2.f };

	ClearBackground({ 75,75,150,255 });

#pragma region camera movement

	static float CAMERA_SPEED = 150.f;
	if (IsKeyDown(KEY_LEFT)) gameData.camera.target.x -= CAMERA_SPEED * GetFrameTime();
	if (IsKeyDown(KEY_RIGHT)) gameData.camera.target.x += CAMERA_SPEED * GetFrameTime();
	if (IsKeyDown(KEY_UP)) gameData.camera.target.y -= CAMERA_SPEED * GetFrameTime();
	if (IsKeyDown(KEY_DOWN)) gameData.camera.target.y += CAMERA_SPEED * GetFrameTime();

	if (GetMouseWheelMove() != 0.f) gameData.camera.zoom += GetMouseWheelMove();

#pragma endregion

	Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), gameData.camera);
	int blockX = (int)floor(worldPos.x);
	int blockY = (int)floor(worldPos.y);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b)
		{
			*b = {};
		}
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		auto b = gameData.gameMap.getBlockSafe(blockX, blockY);
		if (b)
		{
			b->type = editorState.selectedTile;
		}
	}

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
			auto& b = gameData.gameMap.getBlockUnsafe(x, y);

			float size = 1;
			float posX = x * size;
			float posY = y * size;

			//int atlasX = 0;
			//int atlasY = 0;

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

				int atlasX = b.type;
				int atlasY = 0;

				DrawTexturePro(
					assetManager.textures,
					getTextureAtlas(atlasX, atlasY, 32, 32), //source (in sprite)
					{ posX,posY,size,size }, //dest
					{ 0,0 }, //origin (top-left)
					0.f,     //rotation
					WHITE    //tint
				);
			}
		}
	}

	//draw selected block
	DrawTexturePro(
		assetManager.frame,
		{ 0,0,(float)assetManager.frame.width,(float)assetManager.frame.height },
		{ (float)blockX, (float)blockY, 1, 1 },
		{},
		0.f,
		WHITE
	);

	EndMode2D();

#pragma endregion

	DrawFPS(10, 10);

	ImGui::Begin("Camera");

	ImGui::SliderFloat("Camera Zoom: ", &gameData.camera.zoom, 10, 150);
	ImGui::SliderFloat("Camera Speed: ", &CAMERA_SPEED, 5, 30);

	ImGui::End();

	return true;
}

void closeGame()
{
	std::cout << "\n\nCLOSED!!!!!!!!!\n\n";
}