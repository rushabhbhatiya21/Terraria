#pragma once
#include <memory>
#include <cstdint>
#include <unordered_map>
#include <entities/slime.h>

constexpr static std::uint64_t PLAYER_ID = 1;

struct EntityIdHolder
{
	std::uint64_t idCounter = 2;
	
	std::uint64_t getEntityIdAndIncreament();
};

struct EntityHolder
{
	EntityIdHolder idHolder;

	std::unordered_map<std::uint64_t, std::unique_ptr<Entity>> entities;
};