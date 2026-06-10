#pragma once
#include <string>
#include <unordered_map>
#include <raylib.h>
#include "itemIds.h"
#include "../combat/stats.h"

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

enum class WeaponType
{
    NONE,

    SWORD,
    DAGGER,
    SPEAR,

    BOW,
    GUN,

    STAFF,
    WAND
};

enum class DamageType
{
    NONE,
    MELEE,
    RANGED,
    MAGIC,
    SUMMON
};

enum class AttackStyle
{
    NONE,
    SWING,
    THRUST,
    THROW,
    SHOOT,
    CAST
};

enum class ProjectileType
{
    NONE,
    ARROW,
    BULLET,
    ORB
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

// need to add other stats like crit
struct WeaponData
{
    WeaponType type           = WeaponType::NONE;
    DamageType damageType     = DamageType::NONE;
    AttackStyle attackStyle   = AttackStyle::NONE;
    ProjectileType projectile = ProjectileType::NONE;
    OffensiveStats offensive;
};

struct ProjectileData
{
    DamageType damageType = DamageType::NONE;
    OffensiveStats offensive;
    float speed = 0.f;
    float lifetime = 0.f;

    bool affectedByGravity = false;
    bool shouldPassThroughWorld = false;
};

struct ToolData
{
    ToolType type = ToolType::NONE;
    ToolStats tool;
};

struct ConsumableData
{
    int   healAmount = 0;   // HP restored on use (0 if food/other)
    int   manaAmount = 0;   // mana restored on use (0 if not applicable)
};

struct ArmorData
{
    ArmorSlot slot = ArmorSlot::NONE;
    DefensiveStats defensive;
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
    const char*  displayName = "Unknown Item";
    ItemCategory category = ItemCategory::NONE;
    int          maxStack = 1;
    float        useTime = 0.f;

    union
    {
        ToolData       tool;
        WeaponData     weapon;
        ProjectileData projectile;
        ConsumableData consumable;
        ArmorData      armor;
        BlockData      block;
    };

    // ── Constructors ──────────────────────────────────────────────────────────

    // Default (material / block / accessory — no stat block needed)
    ItemDefinition() : category(ItemCategory::NONE), maxStack(0), tool{} {}

    static ItemDefinition makeMaterial(const char* name, float useTime, int maxStack = 999)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::MATERIAL;
        d.maxStack = maxStack;
        d.useTime = useTime;

        return d;
    }

    static ItemDefinition makeAccessory(const char* name, float useTime)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::ACCESSORY;
        d.maxStack = 1;
        d.useTime = useTime;
        return d;
    }

    static ItemDefinition makeTool(const char* name, ToolType type, int power, float useTime, float range)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::TOOL;
        d.maxStack = 1;
        d.useTime = useTime;
        d.tool.type = type;
        d.tool.tool.miningPower = power;
        d.tool.tool.range = range;
        return d;
    }

    static ItemDefinition makeWeapon(const char* name, int damage, int critChance, int critDamage, int armorPen, int knockback, int pierceCount, 
        int range, float useTime, WeaponType weaponType, DamageType damageType, AttackStyle attackStyle, ProjectileType projectile = ProjectileType::NONE)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::WEAPON;
        d.maxStack = 1;
        d.useTime = useTime;
        d.weapon.offensive.damage = damage;
        d.weapon.offensive.critChance = critChance;
        d.weapon.offensive.critDamage = critDamage;
        d.weapon.offensive.armorPen = armorPen;
        d.weapon.offensive.knockback = knockback;
        d.weapon.offensive.pierceCount = pierceCount;
        d.weapon.offensive.range = range;
        d.weapon.type = weaponType;
        d.weapon.damageType = damageType;
        d.weapon.attackStyle = attackStyle;
        d.weapon.projectile = projectile;
        return d;
    }

    static ItemDefinition makeProjectile(const char* name, int damage, int critChance, int critDamage, int armorPen, int knockback, int pierceCount, int range,
        float speed, float lifetime, bool affectedByGravity, bool shouldPassThroughWorld, float useTime)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::PROJECTILE;
        d.maxStack = 999;
        d.useTime = useTime;
        d.projectile.offensive.damage = damage;
        d.projectile.offensive.critChance = critChance;
        d.projectile.offensive.critDamage = critDamage;
        d.projectile.offensive.armorPen = armorPen;
        d.projectile.offensive.knockback = knockback;
        d.projectile.offensive.pierceCount = pierceCount;
        d.projectile.offensive.range = range;
        d.projectile.speed = speed;
        d.projectile.lifetime = lifetime;
        d.projectile.affectedByGravity = affectedByGravity;
        d.projectile.shouldPassThroughWorld = shouldPassThroughWorld;

        return d;
    }

    static ItemDefinition makeArmor(const char* name, ArmorSlot slot, int armor, int maxHealth, int knockbackResist)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::ARMOR;
        d.maxStack = 1;
        d.armor.slot = slot;
        d.armor.defensive.armor = armor;
        d.armor.defensive.maxHealth = maxHealth;
        d.armor.defensive.knockbackResist = knockbackResist;

        return d;
    }

    static ItemDefinition makeConsumable(const char* name, int healAmount, int manaAmount, float useTime, int maxStack = 99)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::CONSUMABLE;
        d.maxStack = maxStack;
        d.useTime = useTime;
        d.consumable.healAmount = healAmount;
        d.consumable.manaAmount = manaAmount;
        return d;
    }

    static ItemDefinition makeBlock(const char* name, BlockType type, int hp, int bestTool, float useTime = 0.15f, int maxStack = 999)
    {
        ItemDefinition d;
        d.displayName = name;
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