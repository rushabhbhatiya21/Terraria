#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>
#include <gameMain.h>

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