#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <raylib.h>
#include "itemIds.h"
#include "../combat/stats.h"
#include <iostream>

namespace Engine
{
    struct Texture;
    struct AssetManager;
}

// ─── Enums ────────────────────────────────────────────────────────────────────

// todo: not using it right now
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
    AMMO,
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
    SUMMON,
    THROWING
};

enum class AttackStyle
{
    NONE,
    SWING,
    THRUST,
    SHOOT,
    CAST,
    THROW
};

enum class AmmoType
{
    NONE,
    ARROW,
    BULLET
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

enum class CollisionType
{
    NONE,
    SOLID,
    PLATFORM
};

enum class ProjectileCollisionType
{
    NONE,
    DESTROY,
    //SLOW,
    //BOOST,
    //RICOCHET
};

// ─── Stat blocks ──────────────────────────────────────────────────────────────

struct WeaponData
{
    WeaponType type           = WeaponType::NONE;
    DamageType damageType     = DamageType::NONE;
    AmmoType requiredAmmo     = AmmoType::NONE;
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

struct AmmoData
{
    AmmoType ammoType;
    ProjectileData projectile;
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
    ItemId              type = Items::air;
    int                   hp = 0;
    uint8_t    lightEmission = 0;
    uint8_t lightAttenuation = 0;
    int            variation = -1;
    ItemId          bestTool = 0;
    CollisionType  collision = CollisionType::NONE;
    ProjectileCollisionType projectileCollision = ProjectileCollisionType::NONE;


    bool isCollidable() const
    {
        return collision != CollisionType::NONE;
    }

    bool isProjectileCollidable() const
    {
        return projectileCollision != ProjectileCollisionType::NONE;
    }
};

// ─── ItemDefinition ───────────────────────────────────────────────────────────

struct ItemDefinition
{
    const char*  displayName = "Unknown Item";
    ItemCategory    category = ItemCategory::NONE;
    AttackStyle  attackStyle = AttackStyle::NONE;
    int             maxStack = 1;
    int              useTime = 60;

    union
    {
        ToolData       tool;
        WeaponData     weapon;
        AmmoData       ammo;
        ConsumableData consumable;
        ArmorData      armor;
        BlockData      block;
    };

    // ── Constructors ──────────────────────────────────────────────────────────

    // Default (material / block / accessory — no stat block needed)
    ItemDefinition() : category(ItemCategory::NONE), maxStack(0), tool{} {}

    static ItemDefinition makeMaterial(const char* name, int maxStack, int useTime)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::MATERIAL;
        d.maxStack = maxStack;
        d.useTime = useTime;

        return d;
    }

    static ItemDefinition makeAccessory(const char* name)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::ACCESSORY;
        d.maxStack = 1;
        return d;
    }

    static ItemDefinition makeTool(const char* name, ToolType type, int power, float range, int useTime, AttackStyle attackStyle = AttackStyle::NONE)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::TOOL;
        d.maxStack = 1;
        d.useTime = useTime;
        d.tool.type = type;
        d.tool.tool.axePower = power;
        d.tool.tool.hammerPower = power;
        d.tool.tool.miningPower = power;
        d.tool.tool.range = (int)range;
        d.attackStyle = attackStyle;
        return d;
    }

    static ItemDefinition makeWeapon(const char* name, int damage, int critChance, int critDamage, int armorPen, int knockback, int pierceCount, 
        int range, int useTime, WeaponType weaponType, DamageType damageType, AttackStyle attackStyle, AmmoType requiredAmmo = AmmoType::NONE)
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
        d.weapon.requiredAmmo = requiredAmmo;
        d.attackStyle = attackStyle;
        return d;
    }

    static ItemDefinition makeAmmo(const char* name, int damage, int critChance, int critDamage, int armorPen, int knockback, 
        int pierceCount, int range, float speed, float lifetime, int useTime, bool affectedByGravity, bool shouldPassThroughWorld, int maxStack, 
        DamageType damageType, AmmoType ammoType = AmmoType::NONE, AttackStyle attackStyle = AttackStyle::NONE)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::AMMO;
        d.attackStyle = attackStyle;
        d.maxStack = maxStack;
        d.useTime = useTime;
        d.ammo.ammoType = ammoType;
        d.ammo.projectile.damageType = damageType;
        d.ammo.projectile.offensive.damage = damage;
        d.ammo.projectile.offensive.critChance = critChance;
        d.ammo.projectile.offensive.critDamage = critDamage;
        d.ammo.projectile.offensive.armorPen = armorPen;
        d.ammo.projectile.offensive.knockback = knockback;
        d.ammo.projectile.offensive.pierceCount = pierceCount;
        d.ammo.projectile.offensive.range = range;
        d.ammo.projectile.speed = speed;
        d.ammo.projectile.lifetime = lifetime;
        d.ammo.projectile.affectedByGravity = affectedByGravity;
        d.ammo.projectile.shouldPassThroughWorld = shouldPassThroughWorld;

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

    static ItemDefinition makeConsumable(const char* name, int healAmount, int manaAmount, int maxStack, int useTime)
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

    static ItemDefinition makeBlock(const char* name, ItemId type, CollisionType collision, ProjectileCollisionType projectileCollision,
        int hp, ItemId bestTool, uint8_t emittedLight, uint8_t attenuation, int maxStack, int useTime)
    {
        ItemDefinition d;
        d.displayName = name;
        d.category = ItemCategory::BLOCK;
        d.maxStack = maxStack;
        d.useTime = useTime;
        d.block.type = type;
        d.block.collision = collision;
        d.block.projectileCollision = projectileCollision;
        d.block.hp = hp;
        d.block.lightEmission = emittedLight;
        d.block.lightAttenuation = attenuation;
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
extern std::vector<ItemDefinition> itemDatabase;

ItemDefinition* getItem(ItemId itemType);
int             getMaxStackSize(ItemId itemId);
bool            isStackable(ItemId itemId);
bool            isItem(ItemId itemType);

// ─── Texture helpers ──────────────────────────────────────────────────────────

const Engine::Texture& getTextureForItemType(int itemType, const Engine::AssetManager& assetManager);
Rectangle getTextureCoordinatesForItemType(int itemType, int cellSizeX = 32, int cellSizeY = 32, bool flipX = false);