#include "item.h"
#include "items/itemIds.h"
#include <helper.h>
#include <assets/texture.h>
#include <assets/assetManager.h>

//using Engine::AssetManager;

std::vector<ItemDefinition> itemDatabase;

// ─── Internal helpers ─────────────────────────────────────────────────────────

ItemDefinition* getItem(ItemId itemId)
{
    return &itemDatabase[(int)itemId];
}

int getMaxStackSize(ItemId itemId)
{
    return itemDatabase[(int)itemId].maxStack;
}

bool isStackable(ItemId itemId)
{
    int maxStackStack = getMaxStackSize(itemId);

    if (maxStackStack == -1)
        return false;

    return maxStackStack > 1;
}

bool isItem(ItemId itemType)
{
    return itemType >= Items::firstItem && itemType < Items::LAST_ITEM;
}

static void reg(ItemId id, ItemDefinition def)
{
    itemDatabase.at(id) = def;
}

const Engine::Texture& getTextureForItemType(int itemType, const Engine::AssetManager& assetManager)
{
    if (!isItem(itemType))
        return assetManager.textures;  // block atlas
    return assetManager.items;         // item atlas
}

Rectangle getTextureCoordinatesForItemType(int itemType, int cellSizeX, int cellSizeY, bool flipX)
{
    if (!isItem(itemType))
        return getTextureAtlas(itemType, 4, cellSizeX, cellSizeY, flipX);
    return getTextureAtlas(itemType - Items::firstItem, 0, cellSizeX, cellSizeY, flipX);
}

// Tool type IDs
constexpr ItemId TOOL_PICKAXE = Items::firstItem;
constexpr ItemId TOOL_AXE = Items::firstItem + 1;
constexpr ItemId TOOL_HAMMER = Items::firstItem + 2;

// ─── Register ─────────────────────────────────────────────────────────────────
//
//  Tool power tiers:    wood=10  stone=14  copper=20  iron=28  gold=40
//  Weapon damage tiers: wood=8   stone=12  copper=16  iron=22  gold=30
//  Weapon useTime:      sword=1.0  spear=1.2  dagger=0.8  shuriken=0.4
//  Weapon knockback:    sword=1.5  spear=1.0  dagger=0.5  shuriken=0.3
//  Weapon range:        sword=2.0  spear=3.0  dagger=1.2  shuriken=1.0
//  Armor defense tiers: copper=2   iron=4     gold=6      ice=5
//

void registerItems()
{
    itemDatabase.resize(Items::LAST_ITEM);
    using D = ItemDefinition;

    // ── Air ──────────────────────────────────────────────────────
    reg(Items::air, D::makeBlock("Air", Items::air, CollisionType::NONE, ProjectileCollisionType::NONE, 0, 0, 0, 1, 0, 0));

    // ── Soil & terrain ──────────────────────────────────────────────────────
    reg(Items::dirt, D::makeBlock("Dirt", Items::dirt, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 20, TOOL_PICKAXE, 0, 4, 999, 10));
    reg(Items::grassBlock, D::makeBlock("Grass Block", Items::grassBlock, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 20, TOOL_PICKAXE, 0, 4, 999, 10));
    reg(Items::stone, D::makeBlock("Stone", Items::stone, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 60, TOOL_PICKAXE, 0, 5, 999, 10));
    reg(Items::grass, D::makeBlock("Grass", Items::grass, CollisionType::NONE, ProjectileCollisionType::NONE, 5, TOOL_AXE, 0, 0, 999, 10));
    reg(Items::sand, D::makeBlock("Sand", Items::sand, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 15, TOOL_PICKAXE, 0, 4, 999, 10));
    reg(Items::sandRuby, D::makeBlock("Ruby Sand", Items::sandRuby, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 80, TOOL_PICKAXE, 0, 6, 999, 10));
    reg(Items::sandStone, D::makeBlock("Sandstone", Items::sandStone, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 55, TOOL_PICKAXE, 0, 5, 999, 10));

    // ── Wood & planks ───────────────────────────────────────────────────────
    reg(Items::woodPlank, D::makeBlock("Wood Plank", Items::woodPlank, CollisionType::SOLID, ProjectileCollisionType::NONE, 35, TOOL_AXE, 0, 1, 999, 10));

    // ── Stone & brick ───────────────────────────────────────────────────────
    reg(Items::stoneBricks, D::makeBlock("Stone Bricks", Items::stoneBricks, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 70, TOOL_HAMMER, 0, 5, 999, 10));

    // ── Soil (cont.) ────────────────────────────────────────────────────────
    reg(Items::clay, D::makeBlock("Clay", Items::clay, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 25, TOOL_PICKAXE, 0, 4, 999, 10));

    // ── Wood (cont.) ────────────────────────────────────────────────────────
    reg(Items::woodLog, D::makeBlock("Wood Log", Items::woodLog, CollisionType::SOLID, ProjectileCollisionType::NONE, 40, TOOL_AXE, 0, 1, 999, 10));
    reg(Items::leaves, D::makeBlock("Leaves", Items::leaves, CollisionType::NONE, ProjectileCollisionType::NONE, 5, TOOL_AXE, 0, 1, 999, 10));

    // ── Ores ────────────────────────────────────────────────────────────────
    reg(Items::copper, D::makeBlock("Copper Ore", Items::copper, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 60, TOOL_PICKAXE, 0, 6, 999, 10));
    reg(Items::iron, D::makeBlock("Iron Ore", Items::iron, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 80, TOOL_PICKAXE, 0, 6, 999, 10));
    reg(Items::gold, D::makeBlock("Gold Ore", Items::gold, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 100, TOOL_PICKAXE, 0, 6, 999, 10));

    // ── Ore blocks ──────────────────────────────────────────────────────────
    reg(Items::copperBlock, D::makeBlock("Copper Block", Items::copperBlock, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 70, TOOL_PICKAXE, 0, 7,  999, 10));
    reg(Items::ironBlock, D::makeBlock("Iron Block", Items::ironBlock, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 90, TOOL_PICKAXE, 0, 7,  999, 10));
    reg(Items::goldBlock, D::makeBlock("Gold Block", Items::goldBlock, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 110, TOOL_PICKAXE, 0, 7,  999, 10));

    // ── Brick ───────────────────────────────────────────────────────────────
    reg(Items::bricks, D::makeBlock("Bricks", Items::bricks, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 65, TOOL_HAMMER, 0, 5,  999, 10));

    // ── Snow & ice ──────────────────────────────────────────────────────────
    reg(Items::snow, D::makeBlock("Snow", Items::snow, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 15, TOOL_PICKAXE, 0, 4, 999, 10));
    reg(Items::ice, D::makeBlock("Ice", Items::ice, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 20, TOOL_PICKAXE, 0, 2, 999, 10));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::rubyBlock, D::makeBlock("Ruby Block", Items::rubyBlock, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 120, TOOL_PICKAXE, 0, 7, 999, 10));

    // ── Furniture / platforms ───────────────────────────────────────────────
    reg(Items::platform, D::makeBlock("Platform", Items::platform, CollisionType::PLATFORM, ProjectileCollisionType::NONE, 20, TOOL_AXE, 0, 1, 999, 10));
    reg(Items::workBench, D::makeBlock("Workbench", Items::workBench, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));

    // ── Special ─────────────────────────────────────────────────────────────
    reg(Items::glass, D::makeBlock("Glass", Items::glass, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 10, TOOL_HAMMER, 0, 1, 999, 10));
    reg(Items::furnace, D::makeBlock("Furnace", Items::furnace, CollisionType::SOLID, ProjectileCollisionType::NONE, 50, TOOL_PICKAXE, 10, 1, 1, 10));
    reg(Items::painting, D::makeBlock("Painting", Items::painting, CollisionType::NONE, ProjectileCollisionType::NONE, 10, TOOL_HAMMER, 0, 1, 999, 10));

    // ── Plants ──────────────────────────────────────────────────────────────
    reg(Items::sappling, D::makeBlock("Sapling", Items::sappling, CollisionType::NONE, ProjectileCollisionType::NONE, 5, TOOL_AXE, 0, 0, 999, 10));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::snowBlueRuby, D::makeBlock("Blue Ruby Snow", Items::snowBlueRuby, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 80, TOOL_PICKAXE, 0, 6, 999, 10));
    reg(Items::blueRubyBlock, D::makeBlock("Blue Ruby Block", Items::blueRubyBlock, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 130, TOOL_PICKAXE, 0, 7, 999, 10));

    // ── Furniture ───────────────────────────────────────────────────────────
    reg(Items::door, D::makeBlock("Door", Items::door, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::jar, D::makeBlock("Jar", Items::jar, CollisionType::NONE, ProjectileCollisionType::NONE, 10, TOOL_HAMMER, 0, 0, 999, 10));
    reg(Items::table, D::makeBlock("Table", Items::table, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::wordrobe, D::makeBlock("Wardrobe", Items::wordrobe, CollisionType::SOLID, ProjectileCollisionType::NONE, 35, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::bookShelf, D::makeBlock("Bookshelf", Items::bookShelf, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));

    // ── Snow brick & furniture ──────────────────────────────────────────────
    reg(Items::snowBricks, D::makeBlock("Snow Bricks", Items::snowBricks, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 70, TOOL_HAMMER, 0, 5, 999, 10));
    reg(Items::iceTable, D::makeBlock("Ice Table", Items::iceTable, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::iceWordrobe, D::makeBlock("Ice Wardrobe", Items::iceWordrobe, CollisionType::SOLID, ProjectileCollisionType::NONE, 35, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::iceBookShelf, D::makeBlock("Ice Bookshelf", Items::iceBookShelf, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::icePlatform, D::makeBlock("Ice Platform", Items::icePlatform, CollisionType::PLATFORM, ProjectileCollisionType::NONE, 20, TOOL_AXE, 0, 1, 999, 10));

    // ── Sand furniture ──────────────────────────────────────────────────────
    reg(Items::sandTable, D::makeBlock("Sand Table", Items::sandTable, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::sandWordrobe, D::makeBlock("Sand Wardrobe", Items::sandWordrobe, CollisionType::SOLID, ProjectileCollisionType::NONE, 35, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::sandBookShelf, D::makeBlock("Sand Bookshelf", Items::sandBookShelf, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::sandPlatform, D::makeBlock("Sand Platform", Items::sandPlatform, CollisionType::PLATFORM, ProjectileCollisionType::NONE, 20, TOOL_AXE, 0, 1, 999, 10));

    // ── Chests ──────────────────────────────────────────────────────────────
    reg(Items::woodenChest, D::makeBlock("Wooden Chest", Items::woodenChest, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 10, TOOL_AXE, 0, 3, 1, 10));
    reg(Items::iceChest, D::makeBlock("Ice Chest", Items::iceChest, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 10, TOOL_AXE, 0, 3, 1, 10));
    reg(Items::sandChest, D::makeBlock("Sand Chest", Items::sandChest, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 10, TOOL_AXE, 0, 3, 1, 10));
    reg(Items::boneChest, D::makeBlock("Bone Chest", Items::boneChest, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 10, TOOL_AXE, 0, 3, 1, 10));

    // ── Bone set ────────────────────────────────────────────────────────────
    reg(Items::boneBricks, D::makeBlock("Bone Bricks", Items::boneBricks, CollisionType::SOLID, ProjectileCollisionType::DESTROY, 75, TOOL_HAMMER, 0, 5, 999, 10));
    reg(Items::boneBench, D::makeBlock("Bone Bench", Items::boneBench, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::boneWordrobe, D::makeBlock("Bone Wardrobe", Items::boneWordrobe, CollisionType::SOLID, ProjectileCollisionType::NONE, 35, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::boneBookShelf, D::makeBlock("Bone Bookshelf", Items::boneBookShelf, CollisionType::SOLID, ProjectileCollisionType::NONE, 30, TOOL_AXE, 0, 2, 1, 10));
    reg(Items::bonePlatform, D::makeBlock("Bone Platform", Items::bonePlatform, CollisionType::PLATFORM, ProjectileCollisionType::NONE, 20, TOOL_AXE, 0, 1, 999, 10));

    // ── Tools ─────────────────────────────────────────────────────────────────
    reg(Items::woodPickaxe, D::makeTool("Wood Pickaxe", ToolType::PICKAXE, 10, 3.f, 60, AttackStyle::SWING));
    reg(Items::woodAxe, D::makeTool("Wood Axe", ToolType::AXE, 10, 3.f, 60, AttackStyle::SWING));
    reg(Items::woodHammer, D::makeTool("Wood Hammer", ToolType::HAMMER, 10, 3.f, 60, AttackStyle::SWING));

    reg(Items::stonePickaxe, D::makeTool("Stone Pickaxe", ToolType::PICKAXE, 14, 3.f, 45, AttackStyle::SWING));
    reg(Items::stoneAxe, D::makeTool("Stone Axe", ToolType::AXE, 14, 3.f, 45, AttackStyle::SWING));
    reg(Items::stoneHammer, D::makeTool("Stone Hammer", ToolType::HAMMER, 14, 3.f, 45, AttackStyle::SWING));

    reg(Items::copperPickaxe, D::makeTool("Copper Pickaxe", ToolType::PICKAXE, 20, 3.f, 30, AttackStyle::SWING));
    reg(Items::copperAxe, D::makeTool("Copper Axe", ToolType::AXE, 20, 3.f, 30, AttackStyle::SWING));
    reg(Items::copperHammer, D::makeTool("Copper Hammer", ToolType::HAMMER, 20, 3.f, 30, AttackStyle::SWING));

    reg(Items::ironPickaxe, D::makeTool("Iron Pickaxe", ToolType::PICKAXE, 28, 3.f, 15, AttackStyle::SWING));
    reg(Items::ironAxe, D::makeTool("Iron Axe", ToolType::AXE, 28, 3.f, 15, AttackStyle::SWING));
    reg(Items::ironHammer, D::makeTool("Iron Hammer", ToolType::HAMMER, 28, 3.f, 15, AttackStyle::SWING));

    reg(Items::goldPickaxe, D::makeTool("Gold Pickaxe", ToolType::PICKAXE, 40, 3.f, 10, AttackStyle::SWING));
    reg(Items::goldAxe, D::makeTool("Gold Axe", ToolType::AXE, 40, 3.f, 10, AttackStyle::SWING));
    reg(Items::goldHammer, D::makeTool("Gold Hammer", ToolType::HAMMER, 40, 3.f, 10, AttackStyle::SWING));

    // ── Materials ─────────────────────────────────────────────────────────────
    reg(Items::slime, D::makeMaterial("Slime", 999, 20));
    reg(Items::copperIngot, D::makeMaterial("Copper Ingot", 999, 20));
    reg(Items::ironIngot, D::makeMaterial("Iron Ingot", 999, 20));
    reg(Items::goldIngot, D::makeMaterial("Gold Ingot", 999, 20));
    reg(Items::ruby, D::makeMaterial("Ruby", 999, 20));
    reg(Items::blueRuby, D::makeMaterial("Blue Ruby", 999, 20));
    reg(Items::paper, D::makeMaterial("Paper", 999, 20));
    reg(Items::book, D::makeMaterial("Book", 999, 20));
    reg(Items::bone, D::makeMaterial("Bone", 999, 20));

    // ── Weapons — swords ─────────────────────────────────────────────────────

    reg(Items::woodenSword,
        D::makeWeapon("Wooden Sword",
            7,      // damage
            4,      // critChance
            150,    // critDamage
            0,      // armorPen
            5,      // knockback
            0,      // pierceCount
            3,      // range
            60,     // useTime
            WeaponType::SWORD, // weaponType
            DamageType::MELEE, // damageType
            AttackStyle::SWING 
        ));

    reg(Items::stoneSword,  D::makeWeapon("Stone Sword",  8, 4, 150, 0, 5, 0, 3, 20, WeaponType::SWORD, DamageType::MELEE, AttackStyle::SWING));
    reg(Items::copperSword, D::makeWeapon("Copper Sword", 9, 4, 150, 0, 6, 0, 3, 21, WeaponType::SWORD, DamageType::MELEE, AttackStyle::SWING));
    reg(Items::ironSword,   D::makeWeapon("Iron Sword",  12, 4, 150, 0, 6, 0, 3, 20, WeaponType::SWORD, DamageType::MELEE, AttackStyle::SWING));
    reg(Items::goldSword,   D::makeWeapon("Gold Sword",  15, 4, 175, 5, 7, 0, 3, 18, WeaponType::SWORD, DamageType::MELEE, AttackStyle::SWING));

    // ── Weapons — spears ─────────────────────────────────────────────────────

    reg(Items::copperSpear, D::makeWeapon("Copper Spear", 8, 4, 150, 1, 6, 5, 5, 31, WeaponType::SPEAR, DamageType::MELEE, AttackStyle::THRUST));
    reg(Items::ironSpear,   D::makeWeapon("Iron Spear",   9, 4, 150, 1, 6, 5, 5, 24, WeaponType::SPEAR, DamageType::MELEE, AttackStyle::THRUST));
    reg(Items::goldSpear,   D::makeWeapon("Gold Spear",  14, 4, 150, 2, 5, 5, 5, 28, WeaponType::SPEAR, DamageType::MELEE, AttackStyle::THRUST));
    reg(Items::iceSpear,    D::makeWeapon("Ice Spear",   12, 4, 150, 3, 5, 5, 5, 20, WeaponType::SPEAR, DamageType::MELEE, AttackStyle::THRUST));

    // ── Weapons — daggers ────────────────────────────────────────────────────
    // todo: make daggers throwable
    reg(Items::copperDagger, D::makeWeapon("Copper Dagger", 12, 8,  175, 0, 1, 0, 2, 18, WeaponType::DAGGER, DamageType::MELEE, AttackStyle::THRUST));
    reg(Items::ironDagger,   D::makeWeapon("Iron Dagger",   14, 8,  175, 0, 2, 0, 2, 15, WeaponType::DAGGER, DamageType::MELEE, AttackStyle::THRUST));
    reg(Items::iceDagger,    D::makeWeapon("Ice Dagger",    14, 12, 200, 0, 1, 0, 2, 14, WeaponType::DAGGER, DamageType::MELEE, AttackStyle::THRUST));

    // ── Weapons — thrown ─────────────────────────────────────────────────────

    reg(Items::shuriken,
        D::makeAmmo("Shuriken",
            10,     // damage
            10,     // critChance
            150,    // critDamage
            0,      // armorPen
            1,      // knockback
            4,      // pierceCount
            8,      // range
            18.0f,  // speed
            5.0f,   // lifetime
            20,     // useTime
            true,   // affectedByGravity
            false,  // shouldPassThroughWorld
            999,    // maxStack
            DamageType::THROWING, // damageType
            AmmoType::NONE,       // ammoType
            AttackStyle::THROW    // attackStyle
        ));

    // ── Consumables ───────────────────────────────────────────────────────────
    reg(Items::healthPotion, D::makeConsumable("Health Potion", 50, 0, 999, 20));
    reg(Items::apple,        D::makeConsumable("Apple",         10, 0, 999, 20));

    // ── Armor — copper ───────────────────────────────────────────────────────

    reg(Items::copperHelmet,
        D::makeArmor("Copper Helmet",
            ArmorSlot::HELMET,
            2,      // armor
            0,      // maxHealth
            2       // knockbackResist
        ));

    reg(Items::copperChestPlate, D::makeArmor("Copper Chestplate", ArmorSlot::CHEST, 3, 5, 4));
    reg(Items::copperBoots, D::makeArmor("Copper Boots", ArmorSlot::BOOTS, 2, 0, 2));

    // ── Armor — iron ─────────────────────────────────────────────────────────

    reg(Items::ironHelmet, D::makeArmor("Iron Helmet", ArmorSlot::HELMET, 3, 0, 3));
    reg(Items::ironChestPlate, D::makeArmor("Iron Chestplate", ArmorSlot::CHEST, 5, 10, 6));
    reg(Items::ironBoots, D::makeArmor("Iron Boots", ArmorSlot::BOOTS, 4, 0, 3));

    // ── Armor — gold ─────────────────────────────────────────────────────────

    reg(Items::goldHelmet, D::makeArmor("Gold Helmet", ArmorSlot::HELMET, 5, 0, 4));
    reg(Items::goldChestPlate, D::makeArmor("Gold Chestplate", ArmorSlot::CHEST, 8, 15, 8));
    reg(Items::goldBoots, D::makeArmor("Gold Boots", ArmorSlot::BOOTS, 5, 0, 4));


    // ── Armor — ice ──────────────────────────────────────────────────────────

    reg(Items::iceHelmet, D::makeArmor("Ice Helmet", ArmorSlot::HELMET, 4, 5, 4));
    reg(Items::iceChestPlate, D::makeArmor("Ice Chestplate", ArmorSlot::CHEST, 6, 15, 6));
    reg(Items::iceBoots, D::makeArmor("Ice Boots", ArmorSlot::BOOTS, 4, 5, 4));

    // ── Accessories ───────────────────────────────────────────────────────────
    reg(Items::partyHat, D::makeAccessory("Party Hat"));
    reg(Items::sunGlasses, D::makeAccessory("Sunglasses"));

    // ── Range — wooden ──────────────────────────────────────────────────────────
    reg(Items::woodenBow,   D::makeWeapon("Wooden Bow", 4, 4, 100, 0, 0, 1, 0, 30, WeaponType::BOW, DamageType::RANGED, AttackStyle::SHOOT, AmmoType::ARROW));
    reg(Items::woodenArrow, D::makeAmmo("Wooden Arrow", 5, 0, 0, 0, 2, 0, 1, 16.f, 5.f, 0, true, false, 999, DamageType::RANGED, AmmoType::ARROW));
}