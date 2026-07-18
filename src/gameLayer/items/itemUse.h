#pragma once
#include <math/vec2.h>

struct Entity;
struct GameMap;
struct ItemStack;
struct Inventory;
struct EntityHolder;
struct ItemDefinition;

void useItem(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, GameMap& gameMap, Engine::Vec2 mouseWorldPos);

void useStyle(Entity* entity, ItemStack& stack, const ItemDefinition& item, EntityHolder& entityHolder, Engine::Vec2 mouseWorldPos);

void useBlock(Entity* entity, const ItemDefinition& item, GameMap& gameMap, Engine::Vec2 mouseWorldPos);

void useArmor(Entity* entity, const ItemDefinition& item, const ItemStack& stack, int index);

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item);