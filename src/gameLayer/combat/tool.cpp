//#include "tool.h"
//#include "entity.h"
//#include <gameMap.h>
//#include <vector>
//#include "bfs.h"
//#include "shake.h"
//#include "particles.h"
//#include "ui/popupText.h"
//#include "items/blocks.h"
//#include "entityHolder.h"
//#include "entities/droppedItem.h"
//
//std::vector<ToolSwing> toolSwings;
//
//void spawnToolSwing(Entity* entity, Vector2 mousePosition, float range, int power)
//{
//	ToolSwing swing{};
//	swing.entity = entity;
//	swing.mousePosition = mousePosition;
//	swing.range = range;
//	swing.power = power;
//
//	swing.lifetime = .4f;
//	swing.direction = !entity->animations.movingLeft;
//
//	toolSwings.push_back(swing);
//}
//
//void updateToolSwing(float deltatime, GameMap& gameMap, EntityHolder& entityHolder, std::vector<Particle>& particles, std::ranlux24_base& rng)
//{
//	for (int i = (int)toolSwings.size() - 1; i >= 0; --i)
//	{
//		ToolSwing& swing = toolSwings[i];
//
//		swing.lifetime -= deltatime;
//
//		if (swing.lifetime <= 0)
//		{
//			swing.lifetime = 0;
//			toolSwings.erase(toolSwings.begin() + i);
//			continue;
//		}
//
//		Vector2i blockPos{
//			(int)swing.mousePosition.x,
//			(int)swing.mousePosition.y
//		};
//
//		auto* b = gameMap.getBlockSafe(blockPos.x, blockPos.y);
//
//		if (b == nullptr || b->type == b->air) continue;
//
//		float dist = Vector2Distance(blockPos.toVector2(), swing.entity->getPosition());
//
//		if (dist <= swing.range)
//		{
//			// todo: if best tool, does double damage
//			// hit block
//			auto brokenType = b->type;
//			if (damageBlock(swing.power, blockPos, *b, particles, rng))
//			{
//				destroyBlock(blockPos, *b, entityHolder);
//
//				// check for tree collapse
//				if (brokenType == b->woodLog || brokenType == b->leaves)
//				{
//					auto res = bfs(blockPos, gameMap);
//
//					if (!res.empty())
//					{
//						for (auto& block : res)
//						{
//							auto* tb = gameMap.getBlockSafe(block.x, block.y);
//							if (!tb) continue;
//							destroyBlock(block, *tb, entityHolder);
//						}
//					}
//				}
//			}
//			swing.lifetime = 0;
//			return;
//		}
//	}
//	return;
//}
//
//bool damageBlock(int power, const Vector2i& blockPos, Block& block, std::vector<Particle>& particles, std::ranlux24_base& rng)
//{
//	triggerShake(blockPos.x, blockPos.y);
//	auto newParticles = spawnParticles({ (float)blockPos.x, (float)blockPos.y }, rng, block.type, 10);
//	particles.insert(particles.end(), newParticles.begin(), newParticles.end());
//
//	spawnPopupText(
//		blockPos.toVector2(),
//		Vector2{ 0, .1f },
//		std::to_string(power),
//		1,
//		.2f,
//		-1.f,
//		WHITE,
//		false
//	);
//
//	block.hp -= power;
//
//	return block.hp <= 0;
//}
//
//void destroyBlock(const Vector2i& blockPos, Block& block, EntityHolder& entityHolder)
//{
//	if (block.type == block.air)
//		return;
//
//	// drop item
//	auto id = entityHolder.idHolder.getEntityIdAndIncreament();
//	auto item = std::make_unique<DroppedItem>();
//	item->teleport(blockPos.center());
//	item->itemType = block.type;
//	item->physics.velocity.y = -3.f;
//
//	DroppedItem* itemPtr = item.get();
//	entityHolder.entities[id] = std::move(item);
//	entityHolder.droppedItems.push_back(itemPtr);
//
//	block = {};
//}
