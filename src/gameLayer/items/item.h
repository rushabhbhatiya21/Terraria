#pragma once
#include <string>
#include <unordered_map>
#include <raylib.h>
#include "items/itemIds.h"

struct AssetManager;

// ─── Enums ────────────────────────────────────────────────────────────────────

enum class ItemCategory
{
    NONE,
    MATERIAL,
    BLOCK,
    TOOL,
    WEAPON,
    ARMOR,
    ACCESSORY,
    CONSUMABLE
};

enum class ToolType
{
    NONE,
    PICKAXE,
    AXE,
    HAMMER
};

enum class ArmorSlot
{
    NONE,
    HELMET,
    CHEST,
    BOOTS
};

// ─── Stat blocks ──────────────────────────────────────────────────────────────

struct WeaponData
{
    int   damage = 0;
    float knockback = 0.f;   // 0 = none, 1 = light, 2 = heavy
    float range = 0.f;
};

struct ToolData
{
    ToolType type = ToolType::NONE;
    int      power = 0;
    float    range = 0.f;
};

struct ConsumableData
{
    int   healAmount = 0;   // HP restored on use (0 if food/other)
    int   manaAmount = 0;   // mana restored on use (0 if not applicable)
};

struct ArmorData
{
    ArmorSlot slot = ArmorSlot::NONE;
    int       defense = 0;  // flat damage reduction
};

// ─── ItemDefinition ───────────────────────────────────────────────────────────

struct ItemDefinition
{
    ItemCategory category = ItemCategory::NONE;
    int          maxStack = 1;
    float        useTime = 0.f;

    union
    {
        ToolData       tool;
        WeaponData     weapon;
        ConsumableData consumable;
        ArmorData      armor;
    };

    // ── Constructors ──────────────────────────────────────────────────────────

    // Default (material / block / accessory — no stat block needed)
    ItemDefinition() : category(ItemCategory::NONE), maxStack(99), tool{} {}

    static ItemDefinition makeMaterial(float useTime, int maxStack = 99)
    {
        ItemDefinition d;
        d.category = ItemCategory::MATERIAL;
        d.maxStack = maxStack;
        d.useTime = useTime;

        return d;
    }

    static ItemDefinition makeAccessory(float useTime)
    {
        ItemDefinition d;
        d.category = ItemCategory::ACCESSORY;
        d.maxStack = 1;
        d.useTime = useTime;
        return d;
    }

    static ItemDefinition makeTool(ToolType type, int power, float useTime, float range)
    {
        ItemDefinition d;
        d.category = ItemCategory::TOOL;
        d.maxStack = 1;
        d.useTime = useTime;
        d.tool.type = type;
        d.tool.power = power;
        d.tool.range = range;
        return d;
    }

    static ItemDefinition makeWeapon(int damage, float useTime, float knockback, float range)
    {
        ItemDefinition d;
        d.category = ItemCategory::WEAPON;
        d.maxStack = 1;
        d.useTime = useTime;
        d.weapon.damage = damage;
        d.weapon.knockback = knockback;
        d.weapon.range = range;
        return d;
    }

    static ItemDefinition makeArmor(ArmorSlot slot, int defense)
    {
        ItemDefinition d;
        d.category = ItemCategory::ARMOR;
        d.maxStack = 1;
        d.armor.slot = slot;
        d.armor.defense = defense;
        return d;
    }

    static ItemDefinition makeConsumable(int healAmount, int manaAmount, float useTime, int maxStack = 20)
    {
        ItemDefinition d;
        d.category = ItemCategory::CONSUMABLE;
        d.maxStack = maxStack;
        d.useTime = useTime;
        d.consumable.healAmount = healAmount;
        d.consumable.manaAmount = manaAmount;
        return d;
    }
};

// ─── ItemStack ────────────────────────────────────────────────────────────────

struct ItemStack
{
    ItemId itemId = 0;
    int    count = 0;
};

// ─── Database ─────────────────────────────────────────────────────────────────

void registerItems();
extern std::unordered_map<ItemId, ItemDefinition> itemDatabase;

ItemDefinition& getItem(int itemType);
bool            isItem(int itemType);

// ─── Texture helpers ──────────────────────────────────────────────────────────

Texture2D getTextureForItemType(int itemType, AssetManager& assetManager);
Rectangle getTextureCoordinatesForItemType(int itemType);