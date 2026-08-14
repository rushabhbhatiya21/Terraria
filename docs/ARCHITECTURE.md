# Architecture Notes

## High-Level Flow

1. Platform/main initializes window + ImGui docking + game loop.
2. Game layer owns assets, gameplay state, menu flow, and runtime updates.
3. Gameplay update advances:
- world time
- player and entities
- input-driven actions
- world/block updates
- rendering passes

## Rendering Pipeline

- Scene renderer uses explicit pass boundaries:
  - Background pass
  - World pass
  - UI pass
- Collector-based API submits typed renderables.
- Backend handles projection and draw submission.

## World Data Model

- World stored in chunk grid (fixed chunk dimensions).
- Block mutations mark dirty regions.
- Dirty render chunks rebuild cached render data.
- Height-map assists sunlight pass computation.

## Lighting Model

- Lighting recalculation algorithms exist (sunlight + local emission propagation).
- Day/night background transitions are active in runtime presentation.
- Current gameplay composition still does not apply full night/cave darkness to final scene output.
- Practical meaning: background mood and transitions are implemented, while lighting-driven full-scene darkness remains a planned visual pass.

## Entity and Combat Model

- Entity base has physics/stats/inventory/equipment hooks.
- Specialized entities implement update/render and serialization routines.
- Combat system applies damage and knockback interactions.
- Projectiles and dropped items are managed through holder maps + typed pointer arrays.

## Content Definitions

- Items use category data contracts (tool, weapon, ammo, armor, block, consumable).
- Recipes map output item to ingredients + station requirement.
- Texture pack loader supports override path fallback to base resources.

## Persistence

- Settings are JSON-backed and auto-saved on change.
- World save/load persists map + player + entity records.
- Entity reconstruction depends on both broad entity type and enemy subtype metadata.
