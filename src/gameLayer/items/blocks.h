#pragma once
#include <cstdint>
#include <algorithm>
#include <asserts.h>
#include <items/item.h>

static constexpr std::uint8_t NO_VARIATION = 255;

struct Block
{
    std::uint16_t type = Items::air;
    std::uint8_t light = 0;
    std::uint8_t sunLight = 0;

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

    const uint8_t getLightEmission() const
    {
        auto* def = getItemDefinition();
        if (!def) return 0;
        return def->block.lightEmission;
    }

    const uint8_t getLightAttenuation() const
    {
        auto* def = getItemDefinition();
        if (!def) return 0;
        return def->block.lightAttenuation;
    }

    void setType(ItemId blockType)
    {
        if (blockType < 0 || blockType >= Items::LAST_ITEM)
            blockType = Items::air;

        type = blockType;
    }

    void setBlockLight(int newLight)
    {
        newLight = (int)std::clamp((float)newLight, 0.f, 15.f);
        light = (uint8_t)newLight;
    }

    void setSunLight(int newSunLight)
    {
        newSunLight = (int)std::clamp((float)newSunLight, 0.f, 15.f);
        sunLight = (uint8_t)newSunLight;
    }

    void clearLight()
    {
        light = 0;
        sunLight = 0;
    }

    void setHp(int newHp)
    {
        if (newHp < 0)
            newHp = 0;
        hp = newHp;
    }

    void clear()
    {
        type = Items::air;
        hp = 0;
        light = 0;
        sunLight = 0;
        variation = NO_VARIATION;
    }
};

inline Block initBlock(ItemId type)
{
    Block b;

    b.clear();

    auto* def = getItem(type);

    permaAssertComment(
        def,
        "Missing item definition in initBlock()"
    );

    b.setType(type);
    b.setHp(def->block.hp);
    b.clearLight();

    return b;
}