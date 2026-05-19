#pragma once

using ItemId = int;

namespace Items
{
    constexpr ItemId air = 0;

    // ── Soil & terrain ──────────────────────────────────────────────────────
    constexpr ItemId dirt = air + 1;
    constexpr ItemId grassBlock = dirt + 1;
    constexpr ItemId stone = grassBlock + 1;
    constexpr ItemId grass = stone + 1;
    constexpr ItemId sand = grass + 1;
    constexpr ItemId sandRuby = sand + 1;
    constexpr ItemId sandStone = sandRuby + 1;

    // ── Wood & planks ───────────────────────────────────────────────────────
    constexpr ItemId woodPlank = sandStone + 1;

    // ── Stone & brick ───────────────────────────────────────────────────────
    constexpr ItemId stoneBricks = woodPlank + 1;

    // ── Soil (cont.) ────────────────────────────────────────────────────────
    constexpr ItemId clay = stoneBricks + 1;

    // ── Wood (cont.) ────────────────────────────────────────────────────────
    constexpr ItemId woodLog = clay + 1;
    constexpr ItemId leaves = woodLog + 1;

    // ── Ores ────────────────────────────────────────────────────────────────
    constexpr ItemId copper = leaves + 1;
    constexpr ItemId iron = copper + 1;
    constexpr ItemId gold = iron + 1;

    // ── Ore blocks ──────────────────────────────────────────────────────────
    constexpr ItemId copperBlock = gold + 1;
    constexpr ItemId ironBlock = copperBlock + 1;
    constexpr ItemId goldBlock = ironBlock + 1;

    // ── Brick ───────────────────────────────────────────────────────────────
    constexpr ItemId bricks = goldBlock + 1;

    // ── Snow & ice ──────────────────────────────────────────────────────────
    constexpr ItemId snow = bricks + 1;
    constexpr ItemId ice = snow + 1;

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    constexpr ItemId rubyBlock = ice + 1;

    // ── Furniture / platforms ───────────────────────────────────────────────
    constexpr ItemId platform = rubyBlock + 1;
    constexpr ItemId workBench = platform + 1;

    // ── Special ─────────────────────────────────────────────────────────────
    constexpr ItemId glass = workBench + 1;
    constexpr ItemId furnace = glass + 1;
    constexpr ItemId painting = furnace + 1;

    // ── Plants ──────────────────────────────────────────────────────────────
    constexpr ItemId sappling = painting + 1;

    // ── Ore blocks (cont.) ──────────────────────────────────────────────────
    constexpr ItemId snowBlueRuby = sappling + 1;
    constexpr ItemId blueRubyBlock = snowBlueRuby + 1;

    // ── Furniture ───────────────────────────────────────────────────────────
    constexpr ItemId door = blueRubyBlock + 1;
    constexpr ItemId jar = door + 1;
    constexpr ItemId table = jar + 1;
    constexpr ItemId wordrobe = table + 1;
    constexpr ItemId bookShelf = wordrobe + 1;

    // ── Snow brick & furniture ──────────────────────────────────────────────
    constexpr ItemId snowBricks = bookShelf + 1;
    constexpr ItemId iceTable = snowBricks + 1;
    constexpr ItemId iceWordrobe = iceTable + 1;
    constexpr ItemId iceBookShelf = iceWordrobe + 1;
    constexpr ItemId icePlatform = iceBookShelf + 1;

    // ── Sand furniture ──────────────────────────────────────────────────────
    constexpr ItemId sandTable = icePlatform + 1;
    constexpr ItemId sandWordrobe = sandTable + 1;
    constexpr ItemId sandBookShelf = sandWordrobe + 1;
    constexpr ItemId sandPlatform = sandBookShelf + 1;

    // ── Chests ──────────────────────────────────────────────────────────────
    constexpr ItemId woodenChest = sandPlatform + 1;
    constexpr ItemId iceChest = woodenChest + 1;
    constexpr ItemId sandChest = iceChest + 1;
    constexpr ItemId boneChest = sandChest + 1;

    // ── Bone set ────────────────────────────────────────────────────────────
    constexpr ItemId boneBricks = boneChest + 1;
    constexpr ItemId boneBench = boneBricks + 1;
    constexpr ItemId boneWordrobe = boneBench + 1;
    constexpr ItemId boneBookShelf = boneWordrobe + 1;
    constexpr ItemId bonePlatform = boneBookShelf + 1;

    // ── Walls ───────────────────────────────────────────────────────────────
    constexpr ItemId dirtWall = bonePlatform + 1;
    constexpr ItemId stoneWall = dirtWall + 1;
    constexpr ItemId woodWall = stoneWall + 1;
    constexpr ItemId sandStoneWall = woodWall + 1;
    constexpr ItemId brickWall = sandStoneWall + 1;
    constexpr ItemId glassWall = brickWall + 1;
    constexpr ItemId copperBlockWall = glassWall + 1;
    constexpr ItemId silverBlockWall = copperBlockWall + 1;
    constexpr ItemId goldBlockWall = silverBlockWall + 1;
    constexpr ItemId snowWall = goldBlockWall + 1;
    constexpr ItemId sandWall = snowWall + 1;
    constexpr ItemId stoneBricksWall = sandWall + 1;
    constexpr ItemId rubyBlockWall = stoneBricksWall + 1;
    constexpr ItemId heroglyphWall = rubyBlockWall + 1;
    constexpr ItemId blueRubyWall = heroglyphWall + 1;
    constexpr ItemId plankedWall = blueRubyWall + 1;
    constexpr ItemId snowBrickWall = plankedWall + 1;
    constexpr ItemId boneBrickWall = snowBrickWall + 1;

    constexpr ItemId LAST_BLOCK = boneBrickWall + 1;

    constexpr ItemId firstItem = 6001;

    constexpr ItemId woodPickaxe = firstItem;
    constexpr ItemId woodAxe = woodPickaxe + 1;
    constexpr ItemId woodHammer = woodAxe + 1;

    constexpr ItemId stonePickaxe = woodHammer + 1;
    constexpr ItemId stoneAxe = stonePickaxe + 1;
    constexpr ItemId stoneHammer = stoneAxe + 1;

    constexpr ItemId copperPickaxe = stoneHammer + 1;
    constexpr ItemId copperAxe = copperPickaxe + 1;
    constexpr ItemId copperHammer = copperAxe + 1;

    constexpr ItemId ironPickaxe = copperHammer + 1;
    constexpr ItemId ironAxe = ironPickaxe + 1;
    constexpr ItemId ironHammer = ironAxe + 1;

    constexpr ItemId goldPickaxe = ironHammer + 1;
    constexpr ItemId goldAxe = goldPickaxe + 1;
    constexpr ItemId goldHammer = goldAxe + 1;

    constexpr ItemId slime = goldHammer + 1;

    constexpr ItemId copperIngot = slime + 1;
    constexpr ItemId ironIngot = copperIngot + 1;
    constexpr ItemId goldIngot = ironIngot + 1;

    constexpr ItemId woodenSword = goldIngot + 1;
    constexpr ItemId stoneSword = woodenSword + 1;
    constexpr ItemId copperSword = stoneSword + 1;
    constexpr ItemId ironSword = copperSword + 1;
    constexpr ItemId goldSword = ironSword + 1;

    constexpr ItemId ruby = goldSword + 1;

    constexpr ItemId copperSpear = ruby + 1;
    constexpr ItemId ironSpear = copperSpear + 1;
    constexpr ItemId goldSpear = ironSpear + 1;

    constexpr ItemId blueRuby = goldSpear + 1;

    constexpr ItemId iceSpear = blueRuby + 1;

    constexpr ItemId healthPotion = iceSpear + 1;
    constexpr ItemId apple = healthPotion + 1;

    constexpr ItemId paper = apple + 1;
    constexpr ItemId book = paper + 1;

    constexpr ItemId bone = book + 1;

    constexpr ItemId shuriken = bone + 1;

    constexpr ItemId copperDagger = shuriken + 1;
    constexpr ItemId ironDagger = copperDagger + 1;
    constexpr ItemId iceDagger = ironDagger + 1;

    constexpr ItemId copperChestPlate = iceDagger + 1;
    constexpr ItemId copperHelmet = copperChestPlate + 1;
    constexpr ItemId copperBoots = copperHelmet + 1;

    constexpr ItemId ironChestPlate = copperBoots + 1;
    constexpr ItemId ironHelmet = ironChestPlate + 1;
    constexpr ItemId ironBoots = ironHelmet + 1;

    constexpr ItemId goldChestPlate = ironBoots + 1;
    constexpr ItemId goldHelmet = goldChestPlate + 1;
    constexpr ItemId goldBoots = goldHelmet + 1;

    constexpr ItemId partyHat = goldBoots + 1;
    constexpr ItemId sunGlasses = partyHat + 1;

    constexpr ItemId iceChestPlate = sunGlasses + 1;
    constexpr ItemId iceHelmet = iceChestPlate + 1;
    constexpr ItemId iceBoots = iceHelmet + 1;

    constexpr ItemId LAST_ITEM = iceBoots + 1;
}