#pragma once

struct Player;
struct ItemStack;
struct ItemDefinition;

void useItem(Player& player, ItemStack& stack, Vector2 mouseWorldPos);

void useWeapon(Player& player, const ItemDefinition& item);

void useTool(Player& player, const ItemDefinition& item, Vector2 mouseWorldPos);

void useBlock(Player& player, const ItemDefinition& item, Vector2 mouseWorldPos);

void useConsumable(Player& player, ItemStack& stack, const ItemDefinition& item);