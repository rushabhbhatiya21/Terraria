#include "item.h"
#include "items/itemIds.h"
#include <helper.h>
#include <assetManager.h>

std::unordered_map<ItemId, ItemDefinition> itemDatabase;

// ─── Internal helpers ─────────────────────────────────────────────────────────

ItemDefinition& getItem(ItemId itemId)
{
    return itemDatabase.at(itemId);
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
    reg(Items::woodenSword, D::makeWeapon(8, 1.0f, 1.5f, 2.0f));
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