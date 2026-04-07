#include <iostream>
#include <vector>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

#include <gameMain.h>
#include <editorState.h>


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

	Texture atlas = LoadTexture(RESOURCES_PATH "textures.png");

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

		int tileSize = 32;
		int atlasW = atlas.width;
		int atlasH = atlas.height;

		//ImGui::Begin("Tiles");
		//int tilesPerRow = atlas.width / tileSize;

		//for (int x = 1; x < tilesPerRow; x++)
		//{
		//	ImGui::PushID(x);
		//	int tileId = x;
		//	int y = 0; // always base row

		//	float u0 = (x * tileSize) / (float)atlas.width;
		//	float v0 = 0.0f;

		//	float u1 = ((x + 1) * tileSize) / (float)atlas.width;
		//	float v1 = (tileSize) / (float)atlas.height;

		//	// decide appearance
		//	bool isSelected = (tileId == editorState.selectedTile);

		//	ImVec4 bgColor = isSelected
		//		? ImVec4(0.8f, 0.8f, 0.0f, 1.0f)   // bright yellow
		//		: ImVec4(0.05f, 0.05f, 0.05f, 1.0f);

		//	ImVec4 tintColor = isSelected
		//		? ImVec4(1.0f, 1.0f, 1.0f, 1.0f)   // full color
		//		: ImVec4(0.3f, 0.3f, 0.3f, 1.0f);  // heavily dimmed

		//	if (ImGui::ImageButton(
		//		(ImTextureID)(intptr_t)atlas.id,
		//		ImVec2(32, 32),
		//		{ u0, v0 },
		//		{ u1, v1 },
		//		-1,
		//		bgColor,
		//		tintColor
		//	))
		//	{
		//		editorState.selectedTile = tileId;
		//	}

		//	ImGui::PopID();
		//	if ((x) % 3 != 0) ImGui::SameLine(); // wrap UI
		//}

		//ImGui::End();

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