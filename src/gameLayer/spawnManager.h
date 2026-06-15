#pragma once
#include <raylib.h>
#include <memory>
#include <type_traits>
#include <entityHolder.h>

struct SpawnManager
{
	template<typename T>
	inline void spawnEnemy(EntityHolder& entityHolder, Vector2 position, Vector2 velocity)
	{
		static_assert(std::is_base_of_v<Enemy, T>);

		auto id = entityHolder.idHolder.getEntityIdAndIncreament();
		auto entity = std::make_unique<T>();

		entity->teleport(position);
		entity->physics.velocity = velocity;

		T* entityPtr = entity.get();
		entityHolder.entities[id] = std::move(entity);
		entityHolder.enemies.push_back(entityPtr);
	}
};