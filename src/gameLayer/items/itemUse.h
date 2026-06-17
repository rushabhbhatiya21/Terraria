#pragma once

struct Entity;
struct GameMap;
struct ItemStack;
struct Inventory;
struct EntityHolder;
struct ItemDefinition;

void useItem(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, GameMap& gameMap, Vector2 mouseWorldPos);

void useStyle(Entity* entity, ItemStack& stack, const ItemDefinition& item, EntityHolder& entityHolder, Vector2 mouseWorldPos);

void useBlock(Entity* entity, const ItemDefinition& item, GameMap& gameMap, Vector2 mouseWorldPos);

void useArmor(Entity* entity, const ItemDefinition& item, const ItemStack& stack, int index);

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item);