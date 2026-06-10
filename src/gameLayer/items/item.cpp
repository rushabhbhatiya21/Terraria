#include "item.h"
#include "items/itemIds.h"
#include <helper.h>
#include <assetManager.h>

std::unordered_map<ItemId, ItemDefinition> itemDatabase;

// ─── Internal helpers ─────────────────────────────────────────────────────────

ItemDefinition* getItem(ItemId itemId)
{
    auto it = itemDatabase.find(itemId);

    if (it != itemDatabase.end())
    {
        return&it->second;
    }

    return nullptr;
}

int getMaxStackSize(ItemId itemId)
{
    ItemDefinition* item = getItem(itemId);
    
    if (!item) return -1;

    return item->maxStack;
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
    itemDatabase[id] = def;
}

Texture2D getTextureForItemType(int itemType, AssetManager& assetManager)
{
    if (!isItem(itemType))
        return assetManager.textures;  // block atlas
    return assetManager.items;         // item atlas
}

Rectangle getTextureCoordinatesForItemType(int itemType)
{
    if (!isItem(itemType))
        return getTextureAtlas(itemType, 4, 32, 32);
    return getTextureAtlas(itemType - Items::firstItem, 0, 32, 32);
}

// Tool type IDs
constexpr int TOOL_PICKAXE = 6001;
constexpr int TOOL_AXE = 6002;
constexpr int TOOL_HAMMER = 6003;

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
    using D = ItemDefinition;

    // ── Soil & terrain ──────────────────────────────────────────────────────
    reg(Items::dirt, D::makeBlock("Dirt", BlockType::dirt, 20, TOOL_PICKAXE, 999, 40));
    reg(Items::grassBlock, D::makeBlock("Grass Block", BlockType::grassBlock, 20, TOOL_PICKAXE, 999, 40));
    reg(Items::stone, D::makeBlock("Stone", BlockType::stone, 60, TOOL_PICKAXE, 999, 40));
    reg(Items::grass, D::makeBlock("Grass", BlockType::grass, 5, TOOL_AXE, 999, 40));
    reg(Items::sand, D::makeBlock("Sand", BlockType::sand, 15, TOOL_PICKAXE, 999, 40));
    reg(Items::sandRuby, D::makeBlock("Ruby Sand", BlockType::sandRuby, 80, TOOL_PICKAXE, 999, 40));
    reg(Items::sandStone, D::makeBlock("Sandstone", BlockType::sandStone, 55, TOOL_PICKAXE, 999, 40));

    // ── Wood & planks ───────────────────────────────────────────────────────
    reg(Items::woodPlank, D::makeBlock("Wood Plank", BlockType::woodPlank, 35, TOOL_AXE, 999, 40));

    // ── Stone & brick ───────────────────────────────────────────────────────
    reg(Items::stoneBricks, D::makeBlock("Stone Bricks", BlockType::stoneBricks, 70, TOOL_HAMMER, 999, 40));

    // ── Soil (cont.) ────────────────────────────────────────────────────────
    reg(Items::clay, D::makeBlock("Clay", BlockType::clay, 25, TOOL_PICKAXE, 999, 40));

    // ── Wood (cont.) ────────────────────────────────────────────────────────
    reg(Items::woodLog, D::makeBlock("Wood Log", BlockType::woodLog, 40, TOOL_AXE, 999, 40));
    reg(Items::leaves, D::makeBlock("Leaves", BlockType::leaves, 5, TOOL_AXE, 999, 40));

    // ── Ores ────────────────────────────────────────────────────────────────
    reg(Items::copper, D::makeBlock("Copper Ore", BlockType::copper, 60, TOOL_PICKAXE, 999, 40));
    reg(Items::iron, D::makeBlock("Iron Ore", BlockType::iron, 80, TOOL_PICKAXE, 999, 40));
    reg(Items::gold, D::makeBlock("Gold Ore", BlockType::gold, 100, TOOL_PICKAXE, 999, 40));

    // ── Ore blocks ──────────────────────────────────────────────────────────
    reg(Items::copperBlock, D::makeBlock("Copper Block", BlockType::copperBlock, 70, TOOL_PICKAXE, 999, 40));
    reg(Items::ironBlock, D::makeBlock("Iron Block", BlockType::ironBlock, 90, TOOL_PICKAXE, 999, 40));
    reg(Items::goldBlock, D::makeBlock("Gold Block", BlockType::goldBlock, 110, TOOL_PICKAXE, 999, 40));

    // ── Brick ───────────────────────────────────────────────────────────────
    reg(Items::bricks, D::makeBlock("Bricks", BlockType::bricks, 65, TOOL_HAMMER, 999, 40));

    // ── Snow & ice ──────────────────────────────────────────────────────────
    reg(Items::snow, D::makeBlock("Snow", BlockType::snow, 15, TOOL_PICKAXE, 999, 40));
    reg(Items::ice, D::makeBlock("Ice", BlockType::ice, 20, TOOL_PICKAXE, 999, 40));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::rubyBlock, D::makeBlock("Ruby Block", BlockType::rubyBlock, 120, TOOL_PICKAXE, 999, 40));

    // ── Furniture / platforms ───────────────────────────────────────────────
    reg(Items::platform, D::makeBlock("Platform", BlockType::platform, 20, TOOL_AXE, 999, 40));
    reg(Items::workBench, D::makeBlock("Workbench", BlockType::workBench, 30, TOOL_AXE, 1, 40));

    // ── Special ─────────────────────────────────────────────────────────────
    reg(Items::glass, D::makeBlock("Glass", BlockType::glass, 10, TOOL_HAMMER, 999, 40));
    reg(Items::furnace, D::makeBlock("Furnace", BlockType::furnace, 50, TOOL_PICKAXE, 1, 40));
    reg(Items::painting, D::makeBlock("Painting", BlockType::painting, 10, TOOL_HAMMER, 999, 40));

    // ── Plants ──────────────────────────────────────────────────────────────
    reg(Items::sappling, D::makeBlock("Sapling", BlockType::sappling, 5, TOOL_AXE, 999, 40));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::snowBlueRuby, D::makeBlock("Blue Ruby Snow", BlockType::snowBlueRuby, 80, TOOL_PICKAXE, 999, 40));
    reg(Items::blueRubyBlock, D::makeBlock("Blue Ruby Block", BlockType::blueRubyBlock, 130, TOOL_PICKAXE, 999, 40));

    // ── Furniture ───────────────────────────────────────────────────────────
    reg(Items::door, D::makeBlock("Door", BlockType::door, 30, TOOL_AXE, 1, 40));
    reg(Items::jar, D::makeBlock("Jar", BlockType::jar, 10, TOOL_HAMMER, 999, 40));
    reg(Items::table, D::makeBlock("Table", BlockType::table, 30, TOOL_AXE, 1, 40));
    reg(Items::wordrobe, D::makeBlock("Wardrobe", BlockType::wordrobe, 35, TOOL_AXE, 1, 40));
    reg(Items::bookShelf, D::makeBlock("Bookshelf", BlockType::bookShelf, 30, TOOL_AXE, 1, 40));

    // ── Snow brick & furniture ──────────────────────────────────────────────
    reg(Items::snowBricks, D::makeBlock("Snow Bricks", BlockType::snowBricks, 70, TOOL_HAMMER, 999, 40));
    reg(Items::iceTable, D::makeBlock("Ice Table", BlockType::iceTable, 30, TOOL_AXE, 1, 40));
    reg(Items::iceWordrobe, D::makeBlock("Ice Wardrobe", BlockType::iceWordrobe, 35, TOOL_AXE, 1, 40));
    reg(Items::iceBookShelf, D::makeBlock("Ice Bookshelf", BlockType::iceBookShelf, 30, TOOL_AXE, 1, 40));
    reg(Items::icePlatform, D::makeBlock("Ice Platform", BlockType::icePlatform, 20, TOOL_AXE, 999, 40));

    // ── Sand furniture ──────────────────────────────────────────────────────
    reg(Items::sandTable, D::makeBlock("Sand Table", BlockType::sandTable, 30, TOOL_AXE, 1, 40));
    reg(Items::sandWordrobe, D::makeBlock("Sand Wardrobe", BlockType::sandWordrobe, 35, TOOL_AXE, 1, 40));
    reg(Items::sandBookShelf, D::makeBlock("Sand Bookshelf", BlockType::sandBookShelf, 30, TOOL_AXE, 1, 40));
    reg(Items::sandPlatform, D::makeBlock("Sand Platform", BlockType::sandPlatform, 20, TOOL_AXE, 999, 40));

    // ── Chests ──────────────────────────────────────────────────────────────
    reg(Items::woodenChest, D::makeBlock("Wooden Chest", BlockType::woodenChest, 40, TOOL_AXE, 1, 40));
    reg(Items::iceChest, D::makeBlock("Ice Chest", BlockType::iceChest, 40, TOOL_AXE, 1, 40));
    reg(Items::sandChest, D::makeBlock("Sand Chest", BlockType::sandChest, 40, TOOL_AXE, 1, 40));
    reg(Items::boneChest, D::makeBlock("Bone Chest", BlockType::boneChest, 40, TOOL_AXE, 1, 40));

    // ── Bone set ────────────────────────────────────────────────────────────
    reg(Items::boneBricks, D::makeBlock("Bone Bricks", BlockType::boneBricks, 75, TOOL_HAMMER, 999, 40));
    reg(Items::boneBench, D::makeBlock("Bone Bench", BlockType::boneBench, 30, TOOL_AXE, 1, 40));
    reg(Items::boneWordrobe, D::makeBlock("Bone Wardrobe", BlockType::boneWordrobe, 35, TOOL_AXE, 1, 40));
    reg(Items::boneBookShelf, D::makeBlock("Bone Bookshelf", BlockType::boneBookShelf, 30, TOOL_AXE, 1, 40));
    reg(Items::bonePlatform, D::makeBlock("Bone Platform", BlockType::bonePlatform, 20, TOOL_AXE, 999, 40));

    // ── Walls (all broken by hammer) ────────────────────────────────────────
    reg(Items::dirtWall,        D::makeBlock("Dirt Wall", BlockType::dirtWall, 12, TOOL_HAMMER, 999, 40));
    reg(Items::stoneWall,       D::makeBlock("Stone Wall", BlockType::stoneWall, 30, TOOL_HAMMER, 999, 40));
    reg(Items::woodWall,        D::makeBlock("Wood Wall", BlockType::woodWall, 20, TOOL_HAMMER, 999, 40));
    reg(Items::sandStoneWall,   D::makeBlock("Sandstone Wall", BlockType::sandStoneWall, 30, TOOL_HAMMER, 999, 40));
    reg(Items::brickWall,       D::makeBlock("Brick Wall", BlockType::brickWall, 35, TOOL_HAMMER, 999, 40));
    reg(Items::glassWall,       D::makeBlock("Glass Wall", BlockType::glassWall, 8, TOOL_HAMMER, 999, 40));
    reg(Items::copperBlockWall, D::makeBlock("Copper Block Wall", BlockType::copperBlockWall, 40, TOOL_HAMMER, 999, 40));
    reg(Items::silverBlockWall, D::makeBlock("Silver Block Wall", BlockType::silverBlockWall, 45, TOOL_HAMMER, 999, 40));
    reg(Items::goldBlockWall,   D::makeBlock("Gold Block Wall", BlockType::goldBlockWall, 55, TOOL_HAMMER, 999, 40));
    reg(Items::snowWall,        D::makeBlock("Snow Wall", BlockType::snowWall, 20, TOOL_HAMMER, 999, 40));
    reg(Items::sandWall,        D::makeBlock("Sand Wall", BlockType::sandWall, 25, TOOL_HAMMER, 999, 40));
    reg(Items::stoneBricksWall, D::makeBlock("Stone Brick Wall", BlockType::stoneBricksWall, 35, TOOL_HAMMER, 999, 40));
    reg(Items::rubyBlockWall,   D::makeBlock("Ruby Block Wall", BlockType::rubyBlockWall, 55, TOOL_HAMMER, 999, 40));
    reg(Items::heroglyphWall,   D::makeBlock("Hieroglyph Wall", BlockType::heroglyphWall, 60, TOOL_HAMMER, 999, 40));
    reg(Items::blueRubyWall,    D::makeBlock("Blue Ruby Wall", BlockType::blueRubyWall, 60, TOOL_HAMMER, 999, 40));
    reg(Items::plankedWall,     D::makeBlock("Planked Wall", BlockType::plankedWall, 20, TOOL_HAMMER, 999, 40));
    reg(Items::snowBrickWall,   D::makeBlock("Snow Brick Wall", BlockType::snowBrickWall, 30, TOOL_HAMMER, 999, 40));
    reg(Items::boneBrickWall,   D::makeBlock("Bone Brick Wall", BlockType::boneBrickWall, 40, TOOL_HAMMER, 999, 40));

    // ── Tools ─────────────────────────────────────────────────────────────────
    reg(Items::woodPickaxe, D::makeTool("Wood Pickaxe", ToolType::PICKAXE, 10, 0.7f, 60, AttackStyle::SWING));
    reg(Items::woodAxe, D::makeTool("Wood Axe", ToolType::AXE, 10, 0.7f, 60, AttackStyle::SWING));
    reg(Items::woodHammer, D::makeTool("Wood Hammer", ToolType::HAMMER, 10, 0.7f, 60, AttackStyle::SWING));

    reg(Items::stonePickaxe, D::makeTool("Stone Pickaxe", ToolType::PICKAXE, 14, 0.7f, 45, AttackStyle::SWING));
    reg(Items::stoneAxe, D::makeTool("Stone Axe", ToolType::AXE, 14, 0.7f, 45, AttackStyle::SWING));
    reg(Items::stoneHammer, D::makeTool("Stone Hammer", ToolType::HAMMER, 14, 0.7f, 45, AttackStyle::SWING));

    reg(Items::copperPickaxe, D::makeTool("Copper Pickaxe", ToolType::PICKAXE, 20, 0.7f, 30, AttackStyle::SWING));
    reg(Items::copperAxe, D::makeTool("Copper Axe", ToolType::AXE, 20, 0.7f, 30, AttackStyle::SWING));
    reg(Items::copperHammer, D::makeTool("Copper Hammer", ToolType::HAMMER, 20, 0.7f, 30, AttackStyle::SWING));

    reg(Items::ironPickaxe, D::makeTool("Iron Pickaxe", ToolType::PICKAXE, 28, 0.7f, 20, AttackStyle::SWING));
    reg(Items::ironAxe, D::makeTool("Iron Axe", ToolType::AXE, 28, 0.7f, 20, AttackStyle::SWING));
    reg(Items::ironHammer, D::makeTool("Iron Hammer", ToolType::HAMMER, 28, 0.7f, 20, AttackStyle::SWING));

    reg(Items::goldPickaxe, D::makeTool("Gold Pickaxe", ToolType::PICKAXE, 40, 0.7f, 10, AttackStyle::SWING));
    reg(Items::goldAxe, D::makeTool("Gold Axe", ToolType::AXE, 40, 0.7f, 10, AttackStyle::SWING));
    reg(Items::goldHammer, D::makeTool("Gold Hammer", ToolType::HAMMER, 40, 0.7f, 10, AttackStyle::SWING));

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
            20,  // useTime
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
        D::makeProjectile("Shuriken",
            10,     // damage
            10,     // critChance
            175,    // critDamage
            0,      // armorPen
            1,      // knockback
            4,      // pierceCount
            8,      // range
            14.0f,  // speed
            3.0f,   // lifetime
            true,   // affectedByGravity
            false,  // shouldPassThroughWorld
            999     // maxStack
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
    reg(Items::woodenBow,   D::makeWeapon("Wooden Bow", 4, 4, 100, 0, 0, 1, 0, 30, WeaponType::BOW, DamageType::RANGED, AttackStyle::SHOOT, ProjectileType::ARROW));
    reg(Items::woodenArrow, D::makeProjectile("Wooden Arrow", 5, 0, 0, 0, 2, 0, 1, 20.f, 5.f, true, false, 999));
}