#include "recipe.h"
//#include "items/blocks.h"

std::unordered_map<ItemId, Recipes::Recipe> Recipes::all =
{
    // platform
    {Items::platform, {2, {{Items::woodLog, 1}}, Recipes::CraftingStation::NONE}},

    // workbench
    {Items::workBench, {1, {{Items::woodLog, 10}}, Recipes::CraftingStation::NONE}},

    // furnace
    {Items::furnace, {1, {{Items::stone, 20}, {Items::woodLog, 4}}, Recipes::CraftingStation::WORKBENCH}},

    // Smelting
    {Items::copperIngot, {1, {{Items::copper, 3}}, Recipes::CraftingStation::FURNACE}},
    {Items::ironIngot,   {1, {{Items::iron,   3}}, Recipes::CraftingStation::FURNACE}},
    {Items::goldIngot,   {1, {{Items::gold,   3}}, Recipes::CraftingStation::FURNACE}},

    // Basic wooden tools
    {Items::woodPickaxe, {1, {{Items::woodLog, 3}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::woodAxe,     {1, {{Items::woodLog, 1}, {Items::stone, 1}}, Recipes::CraftingStation::WORKBENCH}},

    // Copper tier
    {Items::copperPickaxe, {1, {{Items::copperIngot, 8}, {Items::woodLog, 2}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::copperAxe,     {1, {{Items::copperIngot, 6}, {Items::woodLog, 2}}, Recipes::CraftingStation::WORKBENCH}},

    // Iron tier
    {Items::ironPickaxe, {1, {{Items::ironIngot, 8}, {Items::woodLog, 2}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::ironAxe,     {1, {{Items::ironIngot, 6}, {Items::woodLog, 2}}, Recipes::CraftingStation::WORKBENCH}},

    // Gold tier
    {Items::goldPickaxe, {1, {{Items::goldIngot, 8}, {Items::woodLog, 2}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::goldAxe,     {1, {{Items::goldIngot, 6}, {Items::woodLog, 2}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Swords ───────────────────────────────────────────────────────────────
    {Items::woodenSword,      {1, {{Items::woodLog, 3}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::stoneSword,       {1, {{Items::stone, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::copperSword,      {1, {{Items::copperIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::ironSword,        {1, {{Items::ironIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::goldSword,        {1, {{Items::goldIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Spears ───────────────────────────────────────────────────────────────
    {Items::copperSpear,      {1, {{Items::copperIngot, 6}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::ironSpear,        {1, {{Items::ironIngot, 6}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::goldSpear,        {1, {{Items::goldIngot, 6}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::iceSpear,         {1, {{Items::ice, 6}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Daggers ──────────────────────────────────────────────────────────────
    {Items::copperDagger,     {1, {{Items::copperIngot, 4}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::ironDagger,       {1, {{Items::ironIngot, 4}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::iceDagger,        {1, {{Items::ice, 4}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Bows ───────────────────────────────────────────────────────────────
    {Items::woodenBow,        {1, {{Items::woodLog, 10}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Arrows ───────────────────────────────────────────────────────────────
    {Items::woodenArrow,      {1, {{Items::woodLog, 1}}, Recipes::CraftingStation::WORKBENCH}},


    // ── Thrown ───────────────────────────────────────────────────────────────
    {Items::shuriken,         {5, {{Items::ironIngot, 1}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Armor — Copper ───────────────────────────────────────────────────────
    {Items::copperHelmet,     {1, {{Items::copperIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::copperChestPlate, {1, {{Items::copperIngot, 8}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::copperBoots,      {1, {{Items::copperIngot, 4}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Armor — Iron ─────────────────────────────────────────────────────────
    {Items::ironHelmet,       {1, {{Items::ironIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::ironChestPlate,   {1, {{Items::ironIngot, 8}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::ironBoots,        {1, {{Items::ironIngot, 4}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Armor — Gold ─────────────────────────────────────────────────────────
    {Items::goldHelmet,       {1, {{Items::goldIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::goldChestPlate,   {1, {{Items::goldIngot, 8}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::goldBoots,        {1, {{Items::goldIngot, 5}}, Recipes::CraftingStation::WORKBENCH}},

    // ── Armor — Ice ──────────────────────────────────────────────────────────
    {Items::iceHelmet,        {1, {{Items::ice, 5}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::iceChestPlate,    {1, {{Items::ice, 8}}, Recipes::CraftingStation::WORKBENCH}},
    {Items::iceBoots,         {1, {{Items::ice, 4}}, Recipes::CraftingStation::WORKBENCH}}
};