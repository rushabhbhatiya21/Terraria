#pragma once

struct Player;
struct ItemStack;
struct ItemDefinition;

void useItem(Player& player, ItemStack& stack);

void useWeapon(Player& player, const ItemDefinition& item);

void useTool(Player& player, const ItemDefinition& item);

void useConsumable(Player& player, ItemStack& stack, const ItemDefinition& item);