#pragma once

using ItemId = int;

namespace Items
{
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