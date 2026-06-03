#pragma once
#include <string>
#include <unordered_map>
#include <raylib.h>
#include "itemIds.h"

struct AssetManager;

// ─── Enums ────────────────────────────────────────────────────────────────────

enum class ItemUseContext
{
    NONE,
    WORLD,
    INVENTORY
};

enum class ItemCategory
{
    NONE,
    MATERIAL,
    BLOCK,
    TOOL,
    WEAPON,
    PROJECTILE,
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

enum class BlockType
{
    air = 0,
    dirt,
    grassBlock,
    stone,
    grass,
    sand,
    sandRuby,
    sandStone,
    woodPlank,
    stoneBricks,
    clay,
    woodLog,
    leaves,
    copper,
    iron,
    gold,
    copperBlock,
    ironBlock,
    goldBlock,
    bricks,
    snow,
    ice,
    rubyBlock,
    platform,
    workBench,
    glass,
    furnace,
    painting,
    sappling,
    snowBlueRuby,
    blueRubyBlock,
    door,
    jar,
    table,
    wordrobe,
    bookShelf,
    snowBricks,
    iceTable,
    iceWordrobe,
    iceBookShelf,
    icePlatform,
    sandTable,
    sandWordrobe,
    sandBookShelf,
    sandPlatform,
    woodenChest,
    iceChest,
    sandChest,
    boneChest,
    boneBricks,
    boneBench,
    boneWordrobe,
    boneBookShelf,
    bonePlatform,
    dirtWall,
    stoneWall,
    woodWall,
    sandStoneWall,
    brickWall,
    glassWall,
    copperBlockWall,
    silverBlockWall,
    goldBlockWall,
    snowWall,
    sandWall,
    stoneBricksWall,
    rubyBlockWall,
    heroglyphWall,
    blueRubyWall,
    plankedWall,
    snowBrickWall,
    boneBrickWall,

    BLOCKS_COUNT,
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

struct BlockData
{
    BlockType type = BlockType::air;
    int       hp = 0;
    int       bestTool = 0;
};

// ─── ItemDefinition ───────────────────────────────────────────────────────────

struct ItemDefinition
{
    ItemCategory category = ItemCategory::NONE;
    int          maxStack = 1;
    float        useTime = 0.f;
    bool         isTool = false;
    bool         isWeapon = false;
    bool         isConsumable = false;
    bool         isArmor = false;

    union
    {
        ToolData       tool;
        WeaponData     weapon;
        ConsumableData consumable;
        ArmorData      armor;
        BlockData      block;
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
        d.isTool = true;
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
        d.isWeapon = true;
        return d;
    }

    static ItemDefinition makeProjectile(int damage, float useTime, float knockback, float range)
    {
        ItemDefinition d;
        d.category = ItemCategory::PROJECTILE;
        d.maxStack = 99;
        d.useTime = useTime;
        d.weapon.damage = damage;
        d.weapon.knockback = knockback;
        d.weapon.range = range;
        d.isWeapon = true;
        return d;
    }

    static ItemDefinition makeArmor(ArmorSlot slot, int defense)
    {
        ItemDefinition d;
        d.category = ItemCategory::ARMOR;
        d.maxStack = 1;
        d.armor.slot = slot;
        d.armor.defense = defense;
        d.isArmor = true;
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
        d.isConsumable = true;
        return d;
    }

    static ItemDefinition makeBlock(BlockType type, int hp, int bestTool, float useTime = 0.15f, int maxStack = 999)
    {
        ItemDefinition d;
        d.category = ItemCategory::BLOCK;
        d.maxStack = maxStack;
        d.useTime = useTime;
        d.block.type = type;
        d.block.hp = hp;
        d.block.bestTool = bestTool;
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

ItemDefinition* getItem(ItemId itemType);
int             getMaxStackSize(ItemId itemId);
bool            isStackable(ItemId itemId);
bool            isItem(ItemId itemType);

// ─── Texture helpers ──────────────────────────────────────────────────────────

Texture2D getTextureForItemType(int itemType, AssetManager& assetManager);
Rectangle getTextureCoordinatesForItemType(int itemType);