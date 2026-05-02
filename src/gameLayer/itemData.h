#pragma once
#include "items.h"
#include "blockData.h" // TOOL_PICKAXE / TOOL_AXE / TOOL_HAMMER
#include <asserts.h>

// ─── Category tag ─────────────────────────────────────────────────────────────
enum class ItemCategory
{
    none,       // ingots, quest items, materials — no active use
    tool,       // pickaxe / axe / hammer
    weapon,     // sword / spear / dagger / shuriken
    consumable, // potion, food — used on self
    armor,      // helmet / chestplate / boots
    accessory,  // hat, glasses — worn but no stats (extend later)
};

// ─── Per-category stat blocks ─────────────────────────────────────────────────
struct ToolStats
{
    int toolType;    // TOOL_PICKAXE / TOOL_AXE / TOOL_HAMMER
    int toolDamage;  // block HP removed per swing with the correct tool type
    float timeBetweenMine;
    float range;
};

struct WeaponStats
{
    int   meleeDamage;   // HP removed per hit on an enemy
    float attackSpeed;   // swings per second
    float knockback;     // 0 = none, 1 = light, 2 = heavy
    float timeBetweenAttacks;
    float range;
};

struct ConsumableStats
{
    int healAmount;  // HP restored on use (0 if food/other)
    int manaAmount;  // mana restored on use (0 if not applicable)
};

struct ArmorStats
{
    int defense;     // flat damage reduction
    int slot;        // 0 = helmet, 1 = chestplate, 2 = boots
};

// ─── Unified item data row ────────────────────────────────────────────────────
struct ItemData
{
    ItemCategory category;
    int          maxStack;  // 1 for equipment/tools, up to 99 for materials

    union
    {
        ToolStats       tool;
        WeaponStats     weapon;
        ConsumableStats consumable;
        ArmorStats      armor;
    };
};

// ─── Constexpr factory functions (replace macros, no designated inits) ────────
constexpr ItemData makeTool(int toolType, int toolDamage, float timeBetweenMine, float range)
{
    ItemData d{};
    d.category = ItemCategory::tool;
    d.maxStack = 1;
    d.tool.toolType = toolType;
    d.tool.toolDamage = toolDamage;
    d.tool.timeBetweenMine = timeBetweenMine;
    d.tool.range = range;
    return d;
}

constexpr ItemData makeWeapon(int meleeDamage, float attackSpeed, float knockback, float timeBetweenAttacks, float range)
{
    ItemData d{};
    d.category = ItemCategory::weapon;
    d.maxStack = 1;
    d.weapon.meleeDamage = meleeDamage;
    d.weapon.attackSpeed = attackSpeed;
    d.weapon.knockback = knockback;
    d.weapon.timeBetweenAttacks = timeBetweenAttacks;
    d.weapon.range = range;
    return d;
}

constexpr ItemData makeConsumable(int healAmount, int manaAmount, int maxStack)
{
    ItemData d{};
    d.category = ItemCategory::consumable;
    d.maxStack = maxStack;
    d.consumable.healAmount = healAmount;
    d.consumable.manaAmount = manaAmount;
    return d;
}

constexpr ItemData makeArmor(int defense, int slot)
{
    ItemData d{};
    d.category = ItemCategory::armor;
    d.maxStack = 1;
    d.armor.defense = defense;
    d.armor.slot = slot;
    return d;
}

constexpr ItemData makeMaterial(int maxStack)
{
    ItemData d{};
    d.category = ItemCategory::none;
    d.maxStack = maxStack;
    return d;
}

constexpr ItemData makeAccessory()
{
    ItemData d{};
    d.category = ItemCategory::accessory;
    d.maxStack = 1;
    return d;
}

// ─── Table (one row per item, same order as the enum) ────────────────────────
//
//  Tool damage tiers:     wood=10  stone=14  copper=20  iron=28  gold=40
//  Weapon damage tiers:   wood=8   stone=12  copper=16  iron=22  gold=30
//  Weapon speed:          sword=1.8  spear=1.4  dagger=2.2  shuriken=2.5
//  Weapon knockback:      sword=1.5  spear=1.0  dagger=0.5  shuriken=0.3
//  Armor defense tiers:   copper=2   iron=4     gold=6      ice=5
//  Slot IDs:              0=helmet   1=chestplate  2=boots
//

static const ItemData ITEM_DATA[] =
{
    // ── Tools ────────────────────────────────────────────────────────────────
    /* woodPickaxe      */  makeTool(TOOL_PICKAXE,  10, 0.7f, 3.2f),
    /* woodAxe          */  makeTool(TOOL_AXE,      10, 0.7f, 3.2f),
    /* woodHammer       */  makeTool(TOOL_HAMMER,   10, 0.7f, 3.2f),

    /* stonePickaxe     */  makeTool(TOOL_PICKAXE,  14, 0.7f, 3.2f),
    /* stoneAxe         */  makeTool(TOOL_AXE,      14, 0.7f, 3.2f),
    /* stoneHammer      */  makeTool(TOOL_HAMMER,   14, 0.7f, 3.2f),

    /* copperPickaxe    */  makeTool(TOOL_PICKAXE,  20, 0.7f, 3.2f),
    /* copperAxe        */  makeTool(TOOL_AXE,      20, 0.7f, 3.2f),
    /* copperHammer     */  makeTool(TOOL_HAMMER,   20, 0.7f, 3.2f),

    /* ironPickaxe      */  makeTool(TOOL_PICKAXE,  28, 0.7f, 3.2f),
    /* ironAxe          */  makeTool(TOOL_AXE,      28, 0.7f, 3.2f),
    /* ironHammer       */  makeTool(TOOL_HAMMER,   28, 0.7f, 3.2f),

    /* goldPickaxe      */  makeTool(TOOL_PICKAXE,  40, 0.7f, 3.2f),
    /* goldAxe          */  makeTool(TOOL_AXE,      40, 0.7f, 3.2f),
    /* goldHammer       */  makeTool(TOOL_HAMMER,   40, 0.7f, 3.2f),

    // ── Materials ────────────────────────────────────────────────────────────
    /* slime            */  makeMaterial(99),
    /* copperIngot      */  makeMaterial(99),
    /* ironIngot        */  makeMaterial(99),
    /* goldIngot        */  makeMaterial(99),

    // ── Weapons — swords (medium speed, good knockback) ──────────────────────
    /* woodenSword      */  makeWeapon(8, 1.8f, 1.5f, 1.f, 2.f),
    /* stoneSword       */  makeWeapon(12, 1.8f, 1.5f, 1.f, 2.f),
    /* copperSword      */  makeWeapon(16, 1.8f, 1.5f, 1.f, 2.f),
    /* ironSword        */  makeWeapon(22, 1.8f, 1.5f, 1.f, 2.f),
    /* goldSword        */  makeWeapon(30, 1.8f, 1.5f, 1.f, 2.f),

    // ── Materials (cont.) ────────────────────────────────────────────────────
    /* ruby             */  makeMaterial(99),

    // ── Weapons — spears (slower, medium knockback) ───────────────────────────
    /* copperSpear      */  makeWeapon(18, 1.4f, 1.0f, 1.2f, 3.f),
    /* ironSpear        */  makeWeapon(25, 1.4f, 1.0f, 1.2f, 3.f),
    /* goldSpear        */  makeWeapon(34, 1.4f, 1.0f, 1.2f, 3.f),

    // ── Materials (cont.) ────────────────────────────────────────────────────
    /* blueRuby         */  makeMaterial(99),

    // ── Weapons — ice spear ───────────────────────────────────────────────────
    /* iceSpear         */  makeWeapon(28, 1.4f, 1.0f, 1.2f, 3.f),

    // ── Consumables ──────────────────────────────────────────────────────────
    /* healthPotion     */  makeConsumable(50, 0, 20),
    /* apple            */  makeConsumable(10, 0, 20),

    // ── Materials (cont.) ────────────────────────────────────────────────────
    /* paper            */  makeMaterial(99),
    /* book             */  makeMaterial(99),
    /* bone             */  makeMaterial(99),

    // ── Weapons — thrown ─────────────────────────────────────────────────────
    /* shuriken         */  makeWeapon(10, 2.5f, 0.3f, 0.4f, 1.f),

    // ── Weapons — daggers (fast, low knockback) ───────────────────────────────
    /* copperDagger     */  makeWeapon(14, 2.2f, 0.5f, 0.8f, 1.2f),
    /* ironDagger       */  makeWeapon(20, 2.2f, 0.5f, 0.8f, 1.2f),
    /* iceDagger        */  makeWeapon(22, 2.2f, 0.5f, 0.8f, 1.2f),

    // ── Armor — copper (defense 2/piece) ─────────────────────────────────────
    /* copperChestPlate */  makeArmor(2, 1),
    /* copperHelmet     */  makeArmor(2, 0),
    /* copperBoots      */  makeArmor(2, 2),

    // ── Armor — iron (defense 4/piece) ───────────────────────────────────────
    /* ironChestPlate   */  makeArmor(4, 1),
    /* ironHelmet       */  makeArmor(4, 0),
    /* ironBoots        */  makeArmor(4, 2),

    // ── Armor — gold (defense 6/piece) ───────────────────────────────────────
    /* goldChestPlate   */  makeArmor(6, 1),
    /* goldHelmet       */  makeArmor(6, 0),
    /* goldBoots        */  makeArmor(6, 2),

    // ── Accessories ──────────────────────────────────────────────────────────
    /* partyHat         */  makeAccessory(),
    /* sunGlasses       */  makeAccessory(),

    // ── Armor — ice (defense 5/piece) ────────────────────────────────────────
    /* iceChestPlate    */  makeArmor(5, 1),
    /* iceHelmet        */  makeArmor(5, 0),
    /* iceBoots         */  makeArmor(5, 2),
};

static_assert(
    sizeof(ITEM_DATA) / sizeof(ITEM_DATA[0]) == Item::LAST_ITEM - Item::firstItem,
    "ITEM_DATA size mismatch — add or remove an entry to match the Item enum"
);

// ─── Accessors ────────────────────────────────────────────────────────────────

inline const ItemData& getItemData(int itemType)
{
    return ITEM_DATA[itemType - Item::firstItem];
}

// ─── Interaction helpers ──────────────────────────────────────────────────────

// Block damage per swing. Returns 2 for bare hand, 0 for non-tools.
inline int calcBlockDamage(const Block& block, int itemType)
{
    if (itemType < Item::firstItem || itemType >= Item::LAST_ITEM)
        return 2; // bare hand

    const auto& data = getItemData(itemType);
    if (data.category != ItemCategory::tool)
        return 0;

    if (data.tool.toolType == block.bestTool)
        return data.tool.toolDamage;        // correct tool — full damage

    return data.tool.toolDamage / 2;        // wrong tool — half damage
}

inline const float getResetTime(int itemType)
{
    if (itemType < Item::firstItem || itemType >= Item::LAST_ITEM)
        return 0.7f; // bare hand

    const auto& data = getItemData(itemType);

    if (data.category == ItemCategory::tool)
        return data.tool.timeBetweenMine;

    if (data.category == ItemCategory::weapon)
        return data.weapon.timeBetweenAttacks;

    return 0.0f; // fallback
}

// Enemy damage per swing. Returns 0 for non-weapons.
inline int calcMeleeDamage(int itemType)
{
    if (itemType < Item::firstItem || itemType >= Item::LAST_ITEM)
        return 2; // bare hand

    const auto& data = getItemData(itemType);
    if (data.category != ItemCategory::weapon)
        return 0;

    return data.weapon.meleeDamage;
}

inline bool isInRange(int itemType, float dist)
{
    if (itemType < Item::firstItem || itemType >= Item::LAST_ITEM)
        return 1.f; // bare hand

    const auto& data = getItemData(itemType);

    if (data.category == ItemCategory::tool)
        return data.tool.range >= dist;

    if (data.category == ItemCategory::weapon)
        return data.weapon.range >= dist;

    return false;
}

// HP/mana restored on use. Returns {0,0} for non-consumables.
inline ConsumableStats calcConsumable(int itemType)
{
    if (itemType < Item::firstItem || itemType >= Item::LAST_ITEM)
        return { 0, 0 };

    const auto& data = getItemData(itemType);
    if (data.category != ItemCategory::consumable)
        return { 0, 0 };

    return data.consumable;
}

// Total defense from all equipped armor slots.
// equippedItems: array of 3 item types [helmet, chestplate, boots], 0 = empty slot.
inline int calcTotalDefense(const int equippedItems[3])
{
    int total = 0;
    for (int i = 0; i < 3; i++)
    {
        int type = equippedItems[i];
        if (type < Item::firstItem || type >= Item::LAST_ITEM) continue;
        const auto& data = getItemData(type);
        if (data.category == ItemCategory::armor)
            total += data.armor.defense;
    }
    return total;
}