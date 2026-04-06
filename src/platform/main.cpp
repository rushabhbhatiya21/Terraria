#include <iostream>
#include <vector>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

#include <gameMain.h>

//Texture2D atlas = LoadTexture(RESOURCES_PATH "textures.png");
//
//ImVec2 getUV0(int x, int y, int tileSize, int atlasW, int atlasH)
//{
//	return ImVec2(
//		(x * tileSize) / (float)atlasW,
//		(y * tileSize) / (float)atlasH
//	);
//}
//
//ImVec2 getUV1(int x, int y, int tileSize, int atlasW, int atlasH)
//{
//	return ImVec2(
//		((x + 1) * tileSize) / (float)atlasW,
//		((y + 1) * tileSize) / (float)atlasH
//	);
//}

int main()
{
#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE); //no log output to the console by raylib
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 600, "First Window");
	//SetExitKey(KEY_NULL);
	SetTargetFPS(240);

#pragma region imgui config
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 2.5;

#pragma endregion

	if (!initGame())
	{
		return 0; 
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

#pragma region imgui setup
		rlImGuiBegin();

		//imgui window docking
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion

#pragma region imgui windows

		//struct Item {
		//	const char* name;
		//	Texture tex;
		//};


		//int tileSize = 32;
		//int atlasW = atlas.width;
		//int atlasH = atlas.height;

		ImGui::Begin("Tiles");

		//for (int y = 0; y < 3; y++)
		//{
		//	for (int x = 0; x < 3; x++)
		//	{
		//		ImVec2 uv0 = getUV0(x, y, tileSize, atlasW, atlasH);
		//		ImVec2 uv1 = getUV1(x, y, tileSize, atlasW, atlasH);

		//		if (ImGui::ImageButton(
		//			(ImTextureID)&atlas,   // rlImGui uses Texture2D*
		//			ImVec2(32, 32),
		//			uv0,
		//			uv1
		//		))
		//		{
		//			// clicked tile (x, y)
		//		}

		//		ImGui::SameLine();
		//	}
		//}

		ImGui::End();

#pragma endregion

		if (!updateGame())
		{
			CloseWindow();
		}

#pragma region imgui end
		rlImGuiEnd();
#pragma endregion

		EndDrawing();
	}

#pragma region imgui shutdown
	rlImGuiShutdown();
#pragma endregion

	CloseWindow();

	closeGame();

	return 0;
}