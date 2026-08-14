# Features

## Gameplay Systems

- Procedural terrain generation with biome-aware logic.
- Mining and block placement loops.
- Itemized inventory with stack handling.
- Hotbar-based equipment usage.
- Crafting recipes with station gating.
- Enemy encounters and combat feedback.
- Boss fight state machine (Eye phases + summons).
- Gear progression across material tiers.

## World and Rendering Systems

- Chunk-grid world storage.
- Dirty-chunk rebuild workflow for render updates.
- Pass-based scene rendering (background/world/UI).
- Sprite batching path with custom collector/backend split.
- Day/night world-time state with background transitions.
- Lighting data/recalculation systems exist in code, but full night/cave darkness is not currently applied in final gameplay rendering.

## Content Systems

- Item definition model with category-specific stat blocks.
- Weapon/armor/tool/ammo data contracts.
- Recipe table and station requirements.
- Enemy type hierarchy and per-enemy behavior.
- Texture pack override loading workflow.
- Settings persistence (volumes and runtime updates).

## Tooling and Debug

- ImGui debug/control panel.
- In-world selection/copy/paste utilities for structures.
- Runtime texture-pack switching support.
- On-screen performance/debug info hooks.
