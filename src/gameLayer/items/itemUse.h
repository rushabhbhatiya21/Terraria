#pragma once

struct Entity;
struct ItemStack;
struct EntityHolder;
struct ItemDefinition;

void useItem(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, Vector2 mouseWorldPos);

void useWeapon(Entity* entity, const ItemDefinition& item);

void useProjectile(Entity* entity, ItemStack& stack, EntityHolder& entityHolder, Vector2 mouseWorldPos);

void useTool(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos);

void useBlock(Entity* entity, const ItemDefinition& item, Vector2 mouseWorldPos);

void useConsumable(Entity* entity, ItemStack& stack, const ItemDefinition& item);