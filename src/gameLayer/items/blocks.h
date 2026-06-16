#pragma once
#include <cstdint>
#include "items/item.h"

struct Block
{
	std::uint16_t type = 0;
	int variation = -1;

	int hp = 0;
	int light = 0;

    void sanitize()
    {
        if (type >= Items::LAST_BLOCK)
        {
            type = 0;
        }
    }
};