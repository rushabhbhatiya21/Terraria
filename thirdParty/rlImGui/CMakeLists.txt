cmake_minimum_required(VERSION 3.5)
project(rlImGui)

add_library(rlImGui)
target_sources(rlImGui PRIVATE "rlImGui.cpp" "rlImGui.h")
target_include_directories(rlImGui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
target_link_libraries(rlImGui PUBLIC imgui raylib_static)