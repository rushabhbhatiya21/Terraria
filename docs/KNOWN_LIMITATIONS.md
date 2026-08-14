# Known Limitations

This list intentionally focuses on real, observed limitations instead of temporary toggles.

## Gameplay and Combat

1. Attack styles are only partially implemented.
- Implemented: SWING, THROW, SHOOT
- Placeholder/incomplete: THRUST, CAST

2. Swing style physical feel can be improved.
- Current swing logic works, but timing, follow-through, and impact readability still need polish.

3. Progression loop needs stronger structure.
- Core systems exist (crafting/combat/loot), but medium-term goals and reward cadence need tighter design.

## Lighting and Visual Tuning

4. Runtime lighting for night and caves is not currently active in final gameplay rendering.
- Day/night background cycling is implemented and transitions are active.
- World-time/phase updates are implemented.
- Practical result: night visuals now transition correctly in background layers, but cave darkness and full scene darkening are still not visible in gameplay world rendering.

## Save/Load Scope

5. Save/load reliability has been improved, but broader runtime verification is still required.
- Recent consistency fixes were applied for map/entity reconstruction.
- More test coverage is needed for edge cases across long sessions and mixed entity states.

## Content Breadth

6. Boss loot and boss entry flow are still early-stage design.
- Current boss spawn and loot are functional but tuned for development progression and balancing iteration.
