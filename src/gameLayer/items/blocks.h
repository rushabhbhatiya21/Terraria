#pragma once
#include <cstdint>
#include <asserts.h>
#include <items/item.h>

static constexpr std::uint8_t NO_VARIATION = 255;

struct Block
{
    std::uint16_t type = Items::air;
    std::uint8_t light = 0;

    std::uint16_t hp = 0;
    std::uint8_t variation = NO_VARIATION;

    void sanitize()
    {
        if (type >= Items::LAST_BLOCK)
            type = Items::air;
    }

    const ItemDefinition* getItemDefinition() const
    {
        return getItem(type);
    }

    bool isCollidable() const
    {
        auto* def = getItemDefinition();
        return def && def->block.isCollidable();
    }

    bool isProjectileCollidable() const
    {
        auto* def = getItemDefinition();
        return def && def->block.isProjectileCollidable();
    }

    bool isPlatform() const
    {
        auto* def = getItemDefinition();
        return def && def->block.collision == CollisionType::PLATFORM;
    }

    void clear()
    {
        type = Items::air;
        hp = 0;
        light = 0;
    }
};

inline Block initBlock(ItemId type)
{
    Block b;
    b.type = type;

    auto* def = getItem(type);

    permaAssertComment(
        def,
        "Missing item definition in initBlock()"
    );

    b.hp = def->block.hp;
    b.light = 0;

    return b;
}