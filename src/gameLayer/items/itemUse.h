#pragma once

struct Entity;
struct ItemStack;
struct ItemDefinition;

void useItem(Entity* entity, ItemStack& stack, Vector2 mouseWorldPos);

void useWeapon(Entity* entity, const ItemDefinition& item);

void useTool(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos);

void useBlock(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos);

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item);