#include "recipe.h"
#include "items/blocks.h"

std::unordered_map<ItemId, Recipes::Recipe> Recipes::all =
{
    // ── Tools ────────────────────────────────────────────────────────────────
    {Items::woodPickaxe,      {false, 1, {{Block::woodLog, 3}}}},
    {Items::woodAxe,          {false, 1, {{Block::woodLog, 1}, {Block::stone, 1}}}},

    // ── Swords ───────────────────────────────────────────────────────────────
    {Items::woodenSword,      {false, 1, {{Block::woodLog, 3}}}},
    {Items::stoneSword,       {false, 1, {{Block::stone, 5}}}},
    {Items::copperSword,      {false, 1, {{Block::copper, 5}}}},
    {Items::ironSword,        {false, 1, {{Block::iron, 5}}}},
    {Items::goldSword,        {false, 1, {{Block::gold, 5}}}},

    // ── Spears ───────────────────────────────────────────────────────────────
    {Items::copperSpear,      {false, 1, {{Block::copper, 6}}}},
    {Items::ironSpear,        {false, 1, {{Block::iron, 6}}}},
    {Items::goldSpear,        {false, 1, {{Block::gold, 6}}}},
    {Items::iceSpear,         {false, 1, {{Block::ice, 6}}}},

    // ── Daggers ──────────────────────────────────────────────────────────────
    {Items::copperDagger,     {false, 1, {{Block::copper, 4}}}},
    {Items::ironDagger,       {false, 1, {{Block::iron, 4}}}},
    {Items::iceDagger,        {false, 1, {{Block::ice, 4}}}},

    // ── Thrown ───────────────────────────────────────────────────────────────
    {Items::shuriken,         {false, 5, {{Block::iron, 1}}}},

    // ── Armor — Copper ───────────────────────────────────────────────────────
    {Items::copperHelmet,     {false, 1, {{Block::copper, 5}}}},
    {Items::copperChestPlate, {false, 1, {{Block::copper, 8}}}},
    {Items::copperBoots,      {false, 1, {{Block::copper, 4}}}},

    // ── Armor — Iron ─────────────────────────────────────────────────────────
    {Items::ironHelmet,       {false, 1, {{Block::iron, 5}}}},
    {Items::ironChestPlate,   {false, 1, {{Block::iron, 8}}}},
    {Items::ironBoots,        {false, 1, {{Block::iron, 4}}}},

    // ── Armor — Gold ─────────────────────────────────────────────────────────
    {Items::goldHelmet,       {false, 1, {{Block::gold, 5}}}},
    {Items::goldChestPlate,   {false, 1, {{Block::gold, 8}}}},
    {Items::goldBoots,        {false, 1, {{Block::gold, 5}}}},

    // ── Armor — Ice ──────────────────────────────────────────────────────────
    {Items::iceHelmet,        {false, 1, {{Block::ice, 5}}}},
    {Items::iceChestPlate,    {false, 1, {{Block::ice, 8}}}},
    {Items::iceBoots,         {false, 1, {{Block::ice, 4}}}},
};