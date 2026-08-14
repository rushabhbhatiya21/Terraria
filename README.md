# Terraframe

Terraria-inspired 2D sandbox action game built in C++ with custom rendering systems on top of Raylib.

This project combines gameplay systems (mining, combat, crafting, enemies, boss encounters) with engine-oriented architecture work (chunked world rendering, render passes, batching, and texture-pack workflows).

Intended profile use: systems-heavy game development project demonstrating practical C++ gameplay and rendering architecture.

## Project Highlights

- Procedural world generation with caves, ore distribution, and biome logic.
- Chunk-based world data and chunk-cached rendering.
- World-time system and contextual background switching.
- Note: gameplay does not currently apply full night/cave darkness lighting in the final composed scene.
- Player movement/combat with multiple weapon categories.
- Inventory + hotbar + drag/drop UI.
- Crafting with station requirements (workbench/furnace).
- Enemy roster and an Eye boss with phase behavior.
- Save/load pipeline and JSON-based settings persistence.

## Current Status

Recent implementation batch:

- F9 boss spawn keybind (development/testing path).
- Eye boss HP tuned down for current progression balance.
- Eye boss loot drop implementation (gear bundle + rare cosmetic chance).
- Enemy auto-spawner re-enabled.
- Audio init/update lifecycle re-enabled.
- Save/load consistency fixes for map and entity reconstruction paths.

Note: the code paths are implemented. A final manual runtime verification pass is recommended before release tagging.

## Media Preview Placeholders

Use this section as your public-facing visual index. Replace placeholders with real captures.

### 1) Gameplay Overview (GIF)

Expected content:
- 10 to 20 seconds
- movement, mining, and one combat action in a single loop

![Gameplay Overview GIF Placeholder](docs/media/01-gameplay-overview.gif)

### 2) World Generation Snapshot (PNG)

Expected content:
- wide screenshot showing terrain profile, cave openings, and biome transition area

![World Generation Screenshot Placeholder](docs/media/02-world-generation.png)

### 3) Inventory + Crafting UI (PNG)

Expected content:
- inventory open, crafting panel open, visible station-dependent recipe list

![Inventory and Crafting Screenshot Placeholder](docs/media/03-inventory-crafting.png)

### 4) Combat and Enemy Encounter (GIF)

Expected content:
- 6 to 12 seconds
- at least one melee or ranged interaction with damage feedback

![Combat Encounter GIF Placeholder](docs/media/04-combat-encounter.gif)

### 5) Boss Encounter (GIF)

Expected content:
- Eye boss phase behavior, movement pattern, and player dodge/attack rhythm

![Boss Encounter GIF Placeholder](docs/media/05-boss-encounter.gif)

### 6) Day/Night Background Comparison (PNG pair)

Expected content:
- same location captured at day and night to show background/state change (not full world lighting)

![Day Screenshot Placeholder](docs/media/06-day.png)
![Night Screenshot Placeholder](docs/media/07-night.png)

### 7) Texture Pack Comparison (PNG pair)

Expected content:
- same scene in default textures and HD texture pack

![Default Texture Screenshot Placeholder](docs/media/08-texture-default.png)
![HD Texture Screenshot Placeholder](docs/media/09-texture-hd.png)

### 8) Architecture Clip (optional short video)

Expected content:
- quick dev-focused clip or narrated sequence showing debug panel and system toggles

[Architecture Video Placeholder](docs/media/10-architecture-demo.mp4)

## Quick Start (Windows, CMake)

### Option A: VS Code CMake Tools

1. Open the workspace root folder.
2. Configure the CMake project.
3. Build the default target.
4. Run the generated executable.

### Option B: CLI

```bash
cmake -S . -B build
cmake --build build --config Release
```

Executable location depends on your generator (Visual Studio, Ninja, etc.).

## Build Release Zip (Ready for GitHub Releases)

Quick commands:

```bash
cmake -S . -B out/build/prod -DPRODUCTION_BUILD:BOOL=ON
cmake --build out/build/prod --config Release --target release_zip
```

This generates a zip (Terraframe.exe + resources/) at:

out/build/prod/Terraframe-0.1.0-windows.zip

For exact end-to-end steps (including cleanup, artifact location checks, and GitHub Release upload), see [Release Guide](docs/RELEASE.md).

## Controls (Current)

- Movement: A / D or Left / Right
- Jump: Space
- Drop through platforms: S or Down + Space
- Attack / Use held item: Left mouse button
- Hotbar select: 1 to 0
- Toggle inventory: Tab
- Toggle crafting panel: C
- Save world: Ctrl + S
- Load world: Ctrl + L
- Toggle debug ImGui: F10
- Spawn Eye boss (debug bind): F9

## Documentation Map

- [Features](docs/FEATURES.md)
- [Controls](docs/CONTROLS.md)
- [Known Limitations](docs/KNOWN_LIMITATIONS.md)
- [Roadmap](docs/ROADMAP.md)
- [Architecture Notes](docs/ARCHITECTURE.md)
- [Media Capture Guide](docs/MEDIA_GUIDE.md)
- [Release Guide](docs/RELEASE.md)

## Third-Party Libraries

- Raylib
- rlImGui + ImGui docking branch
- FastNoiseSIMD
- nlohmann/json
- glad
- freetype

See [CMakeLists.txt](CMakeLists.txt) and thirdParty for integration details.

## Why This Repo Is Useful

If you are learning game-dev systems in C++, this repository is a practical reference for:

- Building gameplay and engine features in the same codebase.
- Iterating from prototypes to cleaner render/data boundaries.
- Managing real-world tradeoffs in architecture, performance, and feature scope.
