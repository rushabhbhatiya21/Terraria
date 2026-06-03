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
    return getMaxStackSize(itemId) > 1;
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
    reg(Items::dirt, D::makeBlock("Dirt", BlockType::dirt, 20, TOOL_PICKAXE));
    reg(Items::grassBlock, D::makeBlock("Grass Block", BlockType::grassBlock, 20, TOOL_PICKAXE));
    reg(Items::stone, D::makeBlock("Stone", BlockType::stone, 60, TOOL_PICKAXE));
    reg(Items::grass, D::makeBlock("Grass", BlockType::grass, 5, TOOL_AXE));
    reg(Items::sand, D::makeBlock("Sand", BlockType::sand, 15, TOOL_PICKAXE));
    reg(Items::sandRuby, D::makeBlock("Ruby Sand", BlockType::sandRuby, 80, TOOL_PICKAXE));
    reg(Items::sandStone, D::makeBlock("Sandstone", BlockType::sandStone, 55, TOOL_PICKAXE));

    // ── Wood & planks ───────────────────────────────────────────────────────
    reg(Items::woodPlank, D::makeBlock("Wood Plank", BlockType::woodPlank, 35, TOOL_AXE));

    // ── Stone & brick ───────────────────────────────────────────────────────
    reg(Items::stoneBricks, D::makeBlock("Stone Bricks", BlockType::stoneBricks, 70, TOOL_HAMMER));

    // ── Soil (cont.) ────────────────────────────────────────────────────────
    reg(Items::clay, D::makeBlock("Clay", BlockType::clay, 25, TOOL_PICKAXE));

    // ── Wood (cont.) ────────────────────────────────────────────────────────
    reg(Items::woodLog, D::makeBlock("Wood Log", BlockType::woodLog, 40, TOOL_AXE));
    reg(Items::leaves, D::makeBlock("Leaves", BlockType::leaves, 5, TOOL_AXE));

    // ── Ores ────────────────────────────────────────────────────────────────
    reg(Items::copper, D::makeBlock("Copper Ore", BlockType::copper, 60, TOOL_PICKAXE));
    reg(Items::iron, D::makeBlock("Iron Ore", BlockType::iron, 80, TOOL_PICKAXE));
    reg(Items::gold, D::makeBlock("Gold Ore", BlockType::gold, 100, TOOL_PICKAXE));

    // ── Ore blocks ──────────────────────────────────────────────────────────
    reg(Items::copperBlock, D::makeBlock("Copper Block", BlockType::copperBlock, 70, TOOL_PICKAXE));
    reg(Items::ironBlock, D::makeBlock("Iron Block", BlockType::ironBlock, 90, TOOL_PICKAXE));
    reg(Items::goldBlock, D::makeBlock("Gold Block", BlockType::goldBlock, 110, TOOL_PICKAXE));

    // ── Brick ───────────────────────────────────────────────────────────────
    reg(Items::bricks, D::makeBlock("Bricks", BlockType::bricks, 65, TOOL_HAMMER));

    // ── Snow & ice ──────────────────────────────────────────────────────────
    reg(Items::snow, D::makeBlock("Snow", BlockType::snow, 15, TOOL_PICKAXE));
    reg(Items::ice, D::makeBlock("Ice", BlockType::ice, 20, TOOL_PICKAXE));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::rubyBlock, D::makeBlock("Ruby Block", BlockType::rubyBlock, 120, TOOL_PICKAXE));

    // ── Furniture / platforms ───────────────────────────────────────────────
    reg(Items::platform, D::makeBlock("Platform", BlockType::platform, 20, TOOL_AXE));
    reg(Items::workBench, D::makeBlock("Workbench", BlockType::workBench, 30, TOOL_AXE));

    // ── Special ─────────────────────────────────────────────────────────────
    reg(Items::glass, D::makeBlock("Glass", BlockType::glass, 10, TOOL_HAMMER));
    reg(Items::furnace, D::makeBlock("Furnace", BlockType::furnace, 50, TOOL_PICKAXE));
    reg(Items::painting, D::makeBlock("Painting", BlockType::painting, 10, TOOL_HAMMER));

    // ── Plants ──────────────────────────────────────────────────────────────
    reg(Items::sappling, D::makeBlock("Sapling", BlockType::sappling, 5, TOOL_AXE));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::snowBlueRuby, D::makeBlock("Blue Ruby Snow", BlockType::snowBlueRuby, 80, TOOL_PICKAXE));
    reg(Items::blueRubyBlock, D::makeBlock("Blue Ruby Block", BlockType::blueRubyBlock, 130, TOOL_PICKAXE));

    // ── Furniture ───────────────────────────────────────────────────────────
    reg(Items::door, D::makeBlock("Door", BlockType::door, 30, TOOL_AXE, 1));
    reg(Items::jar, D::makeBlock("Jar", BlockType::jar, 10, TOOL_HAMMER));
    reg(Items::table, D::makeBlock("Table", BlockType::table, 30, TOOL_AXE));
    reg(Items::wordrobe, D::makeBlock("Wardrobe", BlockType::wordrobe, 35, TOOL_AXE));
    reg(Items::bookShelf, D::makeBlock("Bookshelf", BlockType::bookShelf, 30, TOOL_AXE));

    // ── Snow brick & furniture ──────────────────────────────────────────────
    reg(Items::snowBricks, D::makeBlock("Snow Bricks", BlockType::snowBricks, 70, TOOL_HAMMER));
    reg(Items::iceTable, D::makeBlock("Ice Table", BlockType::iceTable, 30, TOOL_AXE));
    reg(Items::iceWordrobe, D::makeBlock("Ice Wardrobe", BlockType::iceWordrobe, 35, TOOL_AXE));
    reg(Items::iceBookShelf, D::makeBlock("Ice Bookshelf", BlockType::iceBookShelf, 30, TOOL_AXE));
    reg(Items::icePlatform, D::makeBlock("Ice Platform", BlockType::icePlatform, 20, TOOL_AXE));

    // ── Sand furniture ──────────────────────────────────────────────────────
    reg(Items::sandTable, D::makeBlock("Sand Table", BlockType::sandTable, 30, TOOL_AXE));
    reg(Items::sandWordrobe, D::makeBlock("Sand Wardrobe", BlockType::sandWordrobe, 35, TOOL_AXE));
    reg(Items::sandBookShelf, D::makeBlock("Sand Bookshelf", BlockType::sandBookShelf, 30, TOOL_AXE));
    reg(Items::sandPlatform, D::makeBlock("Sand Platform", BlockType::sandPlatform, 20, TOOL_AXE));

    // ── Chests ──────────────────────────────────────────────────────────────
    reg(Items::woodenChest, D::makeBlock("Wooden Chest", BlockType::woodenChest, 40, TOOL_AXE, 1));
    reg(Items::iceChest, D::makeBlock("Ice Chest", BlockType::iceChest, 40, TOOL_AXE, 1));
    reg(Items::sandChest, D::makeBlock("Sand Chest", BlockType::sandChest, 40, TOOL_AXE, 1));
    reg(Items::boneChest, D::makeBlock("Bone Chest", BlockType::boneChest, 40, TOOL_AXE, 1));

    // ── Bone set ────────────────────────────────────────────────────────────
    reg(Items::boneBricks, D::makeBlock("Bone Bricks", BlockType::boneBricks, 75, TOOL_HAMMER));
    reg(Items::boneBench, D::makeBlock("Bone Bench", BlockType::boneBench, 30, TOOL_AXE));
    reg(Items::boneWordrobe, D::makeBlock("Bone Wardrobe", BlockType::boneWordrobe, 35, TOOL_AXE));
    reg(Items::boneBookShelf, D::makeBlock("Bone Bookshelf", BlockType::boneBookShelf, 30, TOOL_AXE));
    reg(Items::bonePlatform, D::makeBlock("Bone Platform", BlockType::bonePlatform, 20, TOOL_AXE));

    // ── Walls (all broken by hammer) ────────────────────────────────────────
    reg(Items::dirtWall, D::makeBlock("Dirt Wall", BlockType::dirtWall, 12, TOOL_HAMMER));
    reg(Items::stoneWall, D::makeBlock("Stone Wall", BlockType::stoneWall, 30, TOOL_HAMMER));
    reg(Items::woodWall, D::makeBlock("Wood Wall", BlockType::woodWall, 20, TOOL_HAMMER));
    reg(Items::sandStoneWall, D::makeBlock("Sandstone Wall", BlockType::sandStoneWall, 30, TOOL_HAMMER));
    reg(Items::brickWall, D::makeBlock("Brick Wall", BlockType::brickWall, 35, TOOL_HAMMER));
    reg(Items::glassWall, D::makeBlock("Glass Wall", BlockType::glassWall, 8, TOOL_HAMMER));
    reg(Items::copperBlockWall, D::makeBlock("Copper Block Wall", BlockType::copperBlockWall, 40, TOOL_HAMMER));
    reg(Items::silverBlockWall, D::makeBlock("Silver Block Wall", BlockType::silverBlockWall, 45, TOOL_HAMMER));
    reg(Items::goldBlockWall, D::makeBlock("Gold Block Wall", BlockType::goldBlockWall, 55, TOOL_HAMMER));
    reg(Items::snowWall, D::makeBlock("Snow Wall", BlockType::snowWall, 20, TOOL_HAMMER));
    reg(Items::sandWall, D::makeBlock("Sand Wall", BlockType::sandWall, 25, TOOL_HAMMER));
    reg(Items::stoneBricksWall, D::makeBlock("Stone Brick Wall", BlockType::stoneBricksWall, 35, TOOL_HAMMER));
    reg(Items::rubyBlockWall, D::makeBlock("Ruby Block Wall", BlockType::rubyBlockWall, 55, TOOL_HAMMER));
    reg(Items::heroglyphWall, D::makeBlock("Hieroglyph Wall", BlockType::heroglyphWall, 60, TOOL_HAMMER));
    reg(Items::blueRubyWall, D::makeBlock("Blue Ruby Wall", BlockType::blueRubyWall, 60, TOOL_HAMMER));
    reg(Items::plankedWall, D::makeBlock("Planked Wall", BlockType::plankedWall, 20, TOOL_HAMMER));
    reg(Items::snowBrickWall, D::makeBlock("Snow Brick Wall", BlockType::snowBrickWall, 30, TOOL_HAMMER));
    reg(Items::boneBrickWall, D::makeBlock("Bone Brick Wall", BlockType::boneBrickWall, 40, TOOL_HAMMER));

    // ── Tools ─────────────────────────────────────────────────────────────────
    reg(Items::woodPickaxe, D::makeTool("Wood Pickaxe", ToolType::PICKAXE, 10, 0.7f, 3.2f));
    reg(Items::woodAxe, D::makeTool("Wood Axe", ToolType::AXE, 10, 0.7f, 3.2f));
    reg(Items::woodHammer, D::makeTool("Wood Hammer", ToolType::HAMMER, 10, 0.7f, 3.2f));

    reg(Items::stonePickaxe, D::makeTool("Stone Pickaxe", ToolType::PICKAXE, 14, 0.7f, 3.2f));
    reg(Items::stoneAxe, D::makeTool("Stone Axe", ToolType::AXE, 14, 0.7f, 3.2f));
    reg(Items::stoneHammer, D::makeTool("Stone Hammer", ToolType::HAMMER, 14, 0.7f, 3.2f));

    reg(Items::copperPickaxe, D::makeTool("Copper Pickaxe", ToolType::PICKAXE, 20, 0.7f, 3.2f));
    reg(Items::copperAxe, D::makeTool("Copper Axe", ToolType::AXE, 20, 0.7f, 3.2f));
    reg(Items::copperHammer, D::makeTool("Copper Hammer", ToolType::HAMMER, 20, 0.7f, 3.2f));

    reg(Items::ironPickaxe, D::makeTool("Iron Pickaxe", ToolType::PICKAXE, 28, 0.7f, 3.2f));
    reg(Items::ironAxe, D::makeTool("Iron Axe", ToolType::AXE, 28, 0.7f, 3.2f));
    reg(Items::ironHammer, D::makeTool("Iron Hammer", ToolType::HAMMER, 28, 0.7f, 3.2f));

    reg(Items::goldPickaxe, D::makeTool("Gold Pickaxe", ToolType::PICKAXE, 40, 0.7f, 3.2f));
    reg(Items::goldAxe, D::makeTool("Gold Axe", ToolType::AXE, 40, 0.7f, 3.2f));
    reg(Items::goldHammer, D::makeTool("Gold Hammer", ToolType::HAMMER, 40, 0.7f, 3.2f));

    // ── Materials ─────────────────────────────────────────────────────────────
    reg(Items::slime, D::makeMaterial("Slime", .2f));
    reg(Items::copperIngot, D::makeMaterial("Copper Ingot", .2f));
    reg(Items::ironIngot, D::makeMaterial("Iron Ingot", .2f));
    reg(Items::goldIngot, D::makeMaterial("Gold Ingot", .2f));
    reg(Items::ruby, D::makeMaterial("Ruby", .2f));
    reg(Items::blueRuby, D::makeMaterial("Blue Ruby", .2f));
    reg(Items::paper, D::makeMaterial("Paper", .2f));
    reg(Items::book, D::makeMaterial("Book", .2f));
    reg(Items::bone, D::makeMaterial("Bone", .2f));

    // ── Weapons — swords ─────────────────────────────────────────────────────
    reg(Items::woodenSword, D::makeWeapon("Wooden Sword", 8, 1.f, 1.5f, 2.0f));
    reg(Items::stoneSword, D::makeWeapon("Stone Sword", 12, 1.0f, 1.5f, 2.0f));
    reg(Items::copperSword, D::makeWeapon("Copper Sword", 16, 1.0f, 1.5f, 2.0f));
    reg(Items::ironSword, D::makeWeapon("Iron Sword", 22, 1.0f, 1.5f, 2.0f));
    reg(Items::goldSword, D::makeWeapon("Gold Sword", 30, 1.0f, 1.5f, 2.0f));

    // ── Weapons — spears ──────────────────────────────────────────────────────
    reg(Items::copperSpear, D::makeWeapon("Copper Spear", 18, 1.2f, 1.0f, 3.0f));
    reg(Items::ironSpear, D::makeWeapon("Iron Spear", 25, 1.2f, 1.0f, 3.0f));
    reg(Items::goldSpear, D::makeWeapon("Gold Spear", 34, 1.2f, 1.0f, 3.0f));
    reg(Items::iceSpear, D::makeWeapon("Ice Spear", 28, 1.2f, 1.0f, 3.0f));

    // ── Weapons — daggers ─────────────────────────────────────────────────────
    reg(Items::copperDagger, D::makeWeapon("Copper Dagger", 14, 0.8f, 0.5f, 1.2f));
    reg(Items::ironDagger, D::makeWeapon("Iron Dagger", 20, 0.8f, 0.5f, 1.2f));
    reg(Items::iceDagger, D::makeWeapon("Ice Dagger", 22, 0.8f, 0.5f, 1.2f));

    // ── Weapons — thrown ──────────────────────────────────────────────────────
    reg(Items::shuriken, D::makeProjectile("Shuriken", 10, 0.4f, 0.3f, 1.0f));

    // ── Consumables ───────────────────────────────────────────────────────────
    reg(Items::healthPotion, D::makeConsumable("Health Potion", 50, 0, 1.5f, 20));
    reg(Items::apple, D::makeConsumable("Apple", 10, 0, 1.0f, 20));

    // ── Armor — copper ────────────────────────────────────────────────────────
    reg(Items::copperHelmet, D::makeArmor("Copper Helmet", ArmorSlot::HELMET, 2));
    reg(Items::copperChestPlate, D::makeArmor("Copper Chestplate", ArmorSlot::CHEST, 2));
    reg(Items::copperBoots, D::makeArmor("Copper Boots", ArmorSlot::BOOTS, 2));

    // ── Armor — iron ──────────────────────────────────────────────────────────
    reg(Items::ironHelmet, D::makeArmor("Iron Helmet", ArmorSlot::HELMET, 4));
    reg(Items::ironChestPlate, D::makeArmor("Iron Chestplate", ArmorSlot::CHEST, 4));
    reg(Items::ironBoots, D::makeArmor("Iron Boots", ArmorSlot::BOOTS, 4));

    // ── Armor — gold ──────────────────────────────────────────────────────────
    reg(Items::goldHelmet, D::makeArmor("Gold Helmet", ArmorSlot::HELMET, 6));
    reg(Items::goldChestPlate, D::makeArmor("Gold Chestplate", ArmorSlot::CHEST, 6));
    reg(Items::goldBoots, D::makeArmor("Gold Boots", ArmorSlot::BOOTS, 6));

    // ── Armor — ice ───────────────────────────────────────────────────────────
    reg(Items::iceHelmet, D::makeArmor("Ice Helmet", ArmorSlot::HELMET, 5));
    reg(Items::iceChestPlate, D::makeArmor("Ice Chestplate", ArmorSlot::CHEST, 5));
    reg(Items::iceBoots, D::makeArmor("Ice Boots", ArmorSlot::BOOTS, 5));

    // ── Accessories ───────────────────────────────────────────────────────────
    reg(Items::partyHat, D::makeAccessory("Party Hat", .2f));
    reg(Items::sunGlasses, D::makeAccessory("Sunglasses", .2f));
}