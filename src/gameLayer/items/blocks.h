#pragma once
#include <cstdint>
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
};