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

bool isItem(int itemType)
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
    reg(Items::dirt, D::makeBlock(BlockType::dirt, 20, TOOL_PICKAXE));
    reg(Items::grassBlock, D::makeBlock(BlockType::grassBlock, 20, TOOL_PICKAXE));
    reg(Items::stone, D::makeBlock(BlockType::stone, 60, TOOL_PICKAXE));
    reg(Items::grass, D::makeBlock(BlockType::grass, 5, TOOL_AXE));
    reg(Items::sand, D::makeBlock(BlockType::sand, 15, TOOL_PICKAXE));
    reg(Items::sandRuby, D::makeBlock(BlockType::sandRuby, 80, TOOL_PICKAXE));
    reg(Items::sandStone, D::makeBlock(BlockType::sandStone, 55, TOOL_PICKAXE));

    // ── Wood & planks ───────────────────────────────────────────────────────
    reg(Items::woodPlank, D::makeBlock(BlockType::woodPlank, 35, TOOL_AXE));

    // ── Stone & brick ───────────────────────────────────────────────────────
    reg(Items::stoneBricks, D::makeBlock(BlockType::stoneBricks, 70, TOOL_HAMMER));

    // ── Soil (cont.) ────────────────────────────────────────────────────────
    reg(Items::clay, D::makeBlock(BlockType::clay, 25, TOOL_PICKAXE));

    // ── Wood (cont.) ────────────────────────────────────────────────────────
    reg(Items::woodLog, D::makeBlock(BlockType::woodLog, 40, TOOL_AXE));
    reg(Items::leaves, D::makeBlock(BlockType::leaves, 5, TOOL_AXE));

    // ── Ores ────────────────────────────────────────────────────────────────
    reg(Items::copper, D::makeBlock(BlockType::copper, 60, TOOL_PICKAXE));
    reg(Items::iron, D::makeBlock(BlockType::iron, 80, TOOL_PICKAXE));
    reg(Items::gold, D::makeBlock(BlockType::gold, 100, TOOL_PICKAXE));

    // ── Ore blocks ──────────────────────────────────────────────────────────
    reg(Items::copperBlock, D::makeBlock(BlockType::copperBlock, 70, TOOL_PICKAXE));
    reg(Items::ironBlock, D::makeBlock(BlockType::ironBlock, 90, TOOL_PICKAXE));
    reg(Items::goldBlock, D::makeBlock(BlockType::goldBlock, 110, TOOL_PICKAXE));

    // ── Brick ───────────────────────────────────────────────────────────────
    reg(Items::bricks, D::makeBlock(BlockType::bricks, 65, TOOL_HAMMER));

    // ── Snow & ice ──────────────────────────────────────────────────────────
    reg(Items::snow, D::makeBlock(BlockType::snow, 15, TOOL_PICKAXE));
    reg(Items::ice, D::makeBlock(BlockType::ice, 20, TOOL_PICKAXE));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::rubyBlock, D::makeBlock(BlockType::rubyBlock, 120, TOOL_PICKAXE));

    // ── Furniture / platforms ───────────────────────────────────────────────
    reg(Items::platform, D::makeBlock(BlockType::platform, 20, TOOL_AXE));
    reg(Items::workBench, D::makeBlock(BlockType::workBench, 30, TOOL_AXE));

    // ── Special ─────────────────────────────────────────────────────────────
    reg(Items::glass, D::makeBlock(BlockType::glass, 10, TOOL_HAMMER));
    reg(Items::furnace, D::makeBlock(BlockType::furnace, 50, TOOL_PICKAXE));
    reg(Items::painting, D::makeBlock(BlockType::painting, 10, TOOL_HAMMER));

    // ── Plants ──────────────────────────────────────────────────────────────
    reg(Items::sappling, D::makeBlock(BlockType::sappling, 5, TOOL_AXE));

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    reg(Items::snowBlueRuby, D::makeBlock(BlockType::snowBlueRuby, 80, TOOL_PICKAXE));
    reg(Items::blueRubyBlock, D::makeBlock(BlockType::blueRubyBlock, 130, TOOL_PICKAXE));

    // ── Furniture ───────────────────────────────────────────────────────────
    reg(Items::door, D::makeBlock(BlockType::door, 30, TOOL_AXE, 1));
    reg(Items::jar, D::makeBlock(BlockType::jar, 10, TOOL_HAMMER));
    reg(Items::table, D::makeBlock(BlockType::table, 30, TOOL_AXE));
    reg(Items::wordrobe, D::makeBlock(BlockType::wordrobe, 35, TOOL_AXE));
    reg(Items::bookShelf, D::makeBlock(BlockType::bookShelf, 30, TOOL_AXE));

    // ── Snow brick & furniture ──────────────────────────────────────────────
    reg(Items::snowBricks, D::makeBlock(BlockType::snowBricks, 70, TOOL_HAMMER));
    reg(Items::iceTable, D::makeBlock(BlockType::iceTable, 30, TOOL_AXE));
    reg(Items::iceWordrobe, D::makeBlock(BlockType::iceWordrobe, 35, TOOL_AXE));
    reg(Items::iceBookShelf, D::makeBlock(BlockType::iceBookShelf, 30, TOOL_AXE));
    reg(Items::icePlatform, D::makeBlock(BlockType::icePlatform, 20, TOOL_AXE));

    // ── Sand furniture ──────────────────────────────────────────────────────
    reg(Items::sandTable, D::makeBlock(BlockType::sandTable, 30, TOOL_AXE));
    reg(Items::sandWordrobe, D::makeBlock(BlockType::sandWordrobe, 35, TOOL_AXE));
    reg(Items::sandBookShelf, D::makeBlock(BlockType::sandBookShelf, 30, TOOL_AXE));
    reg(Items::sandPlatform, D::makeBlock(BlockType::sandPlatform, 20, TOOL_AXE));

    // ── Chests ──────────────────────────────────────────────────────────────
    reg(Items::woodenChest, D::makeBlock(BlockType::woodenChest, 40, TOOL_AXE, 1));
    reg(Items::iceChest, D::makeBlock(BlockType::iceChest, 40, TOOL_AXE, 1));
    reg(Items::sandChest, D::makeBlock(BlockType::sandChest, 40, TOOL_AXE, 1));
    reg(Items::boneChest, D::makeBlock(BlockType::boneChest, 40, TOOL_AXE, 1));

    // ── Bone set ────────────────────────────────────────────────────────────
    reg(Items::boneBricks, D::makeBlock(BlockType::boneBricks, 75, TOOL_HAMMER));
    reg(Items::boneBench, D::makeBlock(BlockType::boneBench, 30, TOOL_AXE));
    reg(Items::boneWordrobe, D::makeBlock(BlockType::boneWordrobe, 35, TOOL_AXE));
    reg(Items::boneBookShelf, D::makeBlock(BlockType::boneBookShelf, 30, TOOL_AXE));
    reg(Items::bonePlatform, D::makeBlock(BlockType::bonePlatform, 20, TOOL_AXE));

    // ── Walls (all broken by hammer) ────────────────────────────────────────
    reg(Items::dirtWall, D::makeBlock(BlockType::dirtWall, 12, TOOL_HAMMER));
    reg(Items::stoneWall, D::makeBlock(BlockType::stoneWall, 30, TOOL_HAMMER));
    reg(Items::woodWall, D::makeBlock(BlockType::woodWall, 20, TOOL_HAMMER));
    reg(Items::sandStoneWall, D::makeBlock(BlockType::sandStoneWall, 30, TOOL_HAMMER));
    reg(Items::brickWall, D::makeBlock(BlockType::brickWall, 35, TOOL_HAMMER));
    reg(Items::glassWall, D::makeBlock(BlockType::glassWall, 8, TOOL_HAMMER));
    reg(Items::copperBlockWall, D::makeBlock(BlockType::copperBlockWall, 40, TOOL_HAMMER));
    reg(Items::silverBlockWall, D::makeBlock(BlockType::silverBlockWall, 45, TOOL_HAMMER));
    reg(Items::goldBlockWall, D::makeBlock(BlockType::goldBlockWall, 55, TOOL_HAMMER));
    reg(Items::snowWall, D::makeBlock(BlockType::snowWall, 20, TOOL_HAMMER));
    reg(Items::sandWall, D::makeBlock(BlockType::sandWall, 25, TOOL_HAMMER));
    reg(Items::stoneBricksWall, D::makeBlock(BlockType::stoneBricksWall, 35, TOOL_HAMMER));
    reg(Items::rubyBlockWall, D::makeBlock(BlockType::rubyBlockWall, 55, TOOL_HAMMER));
    reg(Items::heroglyphWall, D::makeBlock(BlockType::heroglyphWall, 60, TOOL_HAMMER));
    reg(Items::blueRubyWall, D::makeBlock(BlockType::blueRubyWall, 60, TOOL_HAMMER));
    reg(Items::plankedWall, D::makeBlock(BlockType::plankedWall, 20, TOOL_HAMMER));
    reg(Items::snowBrickWall, D::makeBlock(BlockType::snowBrickWall, 30, TOOL_HAMMER));
    reg(Items::boneBrickWall, D::makeBlock(BlockType::boneBrickWall, 40, TOOL_HAMMER));

    // ── Tools ─────────────────────────────────────────────────────────────────
    reg(Items::woodPickaxe, D::makeTool(ToolType::PICKAXE, 10, 0.7f, 3.2f));
    reg(Items::woodAxe, D::makeTool(ToolType::AXE, 10, 0.7f, 3.2f));
    reg(Items::woodHammer, D::makeTool(ToolType::HAMMER, 10, 0.7f, 3.2f));

    reg(Items::stonePickaxe, D::makeTool(ToolType::PICKAXE, 14, 0.7f, 3.2f));
    reg(Items::stoneAxe, D::makeTool(ToolType::AXE, 14, 0.7f, 3.2f));
    reg(Items::stoneHammer, D::makeTool(ToolType::HAMMER, 14, 0.7f, 3.2f));

    reg(Items::copperPickaxe, D::makeTool(ToolType::PICKAXE, 20, 0.7f, 3.2f));
    reg(Items::copperAxe, D::makeTool(ToolType::AXE, 20, 0.7f, 3.2f));
    reg(Items::copperHammer, D::makeTool(ToolType::HAMMER, 20, 0.7f, 3.2f));

    reg(Items::ironPickaxe, D::makeTool(ToolType::PICKAXE, 28, 0.7f, 3.2f));
    reg(Items::ironAxe, D::makeTool(ToolType::AXE, 28, 0.7f, 3.2f));
    reg(Items::ironHammer, D::makeTool(ToolType::HAMMER, 28, 0.7f, 3.2f));

    reg(Items::goldPickaxe, D::makeTool(ToolType::PICKAXE, 40, 0.7f, 3.2f));
    reg(Items::goldAxe, D::makeTool(ToolType::AXE, 40, 0.7f, 3.2f));
    reg(Items::goldHammer, D::makeTool(ToolType::HAMMER, 40, 0.7f, 3.2f));

    // ── Materials ─────────────────────────────────────────────────────────────
    reg(Items::slime, D::makeMaterial(.2f));
    reg(Items::copperIngot, D::makeMaterial(.2f));
    reg(Items::ironIngot, D::makeMaterial(.2f));
    reg(Items::goldIngot, D::makeMaterial(.2f));
    reg(Items::ruby, D::makeMaterial(.2f));
    reg(Items::blueRuby, D::makeMaterial(.2f));
    reg(Items::paper, D::makeMaterial(.2f));
    reg(Items::book, D::makeMaterial(.2f));
    reg(Items::bone, D::makeMaterial(.2f));

    // ── Weapons — swords (damage, useTime, knockback, range) ─────────────────
    reg(Items::woodenSword, D::makeWeapon(8, 1.f, 1.5f, 2.0f));
    reg(Items::stoneSword, D::makeWeapon(12, 1.0f, 1.5f, 2.0f));
    reg(Items::copperSword, D::makeWeapon(16, 1.0f, 1.5f, 2.0f));
    reg(Items::ironSword, D::makeWeapon(22, 1.0f, 1.5f, 2.0f));
    reg(Items::goldSword, D::makeWeapon(30, 1.0f, 1.5f, 2.0f));

    // ── Weapons — spears (slower, longer range) ───────────────────────────────
    reg(Items::copperSpear, D::makeWeapon(18, 1.2f, 1.0f, 3.0f));
    reg(Items::ironSpear, D::makeWeapon(25, 1.2f, 1.0f, 3.0f));
    reg(Items::goldSpear, D::makeWeapon(34, 1.2f, 1.0f, 3.0f));
    reg(Items::iceSpear, D::makeWeapon(28, 1.2f, 1.0f, 3.0f));

    // ── Weapons — daggers (fast, low knockback) ───────────────────────────────
    reg(Items::copperDagger, D::makeWeapon(14, 0.8f, 0.5f, 1.2f));
    reg(Items::ironDagger, D::makeWeapon(20, 0.8f, 0.5f, 1.2f));
    reg(Items::iceDagger, D::makeWeapon(22, 0.8f, 0.5f, 1.2f));

    // ── Weapons — thrown ──────────────────────────────────────────────────────
    reg(Items::shuriken, D::makeWeapon(10, 0.4f, 0.3f, 1.0f));

    // ── Consumables (healAmount, manaAmount, useTime, maxStack) ───────────────
    reg(Items::healthPotion, D::makeConsumable(50, 0, 1.5f, 20));
    reg(Items::apple, D::makeConsumable(10, 0, 1.0f, 20));

    // ── Armor — copper (defense, slot) ────────────────────────────────────────
    reg(Items::copperHelmet, D::makeArmor(ArmorSlot::HELMET, 2));
    reg(Items::copperChestPlate, D::makeArmor(ArmorSlot::CHEST, 2));
    reg(Items::copperBoots, D::makeArmor(ArmorSlot::BOOTS, 2));

    // ── Armor — iron ──────────────────────────────────────────────────────────
    reg(Items::ironHelmet, D::makeArmor(ArmorSlot::HELMET, 4));
    reg(Items::ironChestPlate, D::makeArmor(ArmorSlot::CHEST, 4));
    reg(Items::ironBoots, D::makeArmor(ArmorSlot::BOOTS, 4));

    // ── Armor — gold ──────────────────────────────────────────────────────────
    reg(Items::goldHelmet, D::makeArmor(ArmorSlot::HELMET, 6));
    reg(Items::goldChestPlate, D::makeArmor(ArmorSlot::CHEST, 6));
    reg(Items::goldBoots, D::makeArmor(ArmorSlot::BOOTS, 6));

    // ── Armor — ice ───────────────────────────────────────────────────────────
    reg(Items::iceHelmet, D::makeArmor(ArmorSlot::HELMET, 5));
    reg(Items::iceChestPlate, D::makeArmor(ArmorSlot::CHEST, 5));
    reg(Items::iceBoots, D::makeArmor(ArmorSlot::BOOTS, 5));

    // ── Accessories ───────────────────────────────────────────────────────────
    reg(Items::partyHat, D::makeAccessory(.2f));
    reg(Items::sunGlasses, D::makeAccessory(.2f));
}