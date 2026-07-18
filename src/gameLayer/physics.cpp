#pragma once
#include <physics.h>
#include <gameMap.h>
#include "items/item.h"

void PhysicalEntity::updatePhysicsTimers(float deltaTime)
{
	dropThroughTimer -= deltaTime;

	if (dropThroughTimer < 0.f)
		dropThroughTimer = 0.f;
}


void PhysicalEntity::resolveConstrains(GameMap& mapData) 
{
	upTouch = false;
	downTouch = false;
	leftTouch = false;
	rightTouch = false;
	standingOnPlatform = false;

	Engine::Vec2& pos = transform.pos;

	float distance = Engine::Vec2Distance(pos, lastPosition);

	if (distance == 0)
	{
		// no movement
		return;
	}

	float GRANULARITY = 0.8f;

	if (distance <= GRANULARITY)
	{
		checkCollisionOnce(mapData, pos);
	}
	else
	{
		Engine::Vec2 newPos = lastPosition;
		Engine::Vec2 delta = pos - lastPosition;
		delta = Engine::Vec2Normalize(delta);
		delta *= GRANULARITY * 0.99f;

		do
		{
			newPos += delta;
			Engine::Vec2 posTest = newPos;
			checkCollisionOnce(mapData, newPos);

			// check if checkCollisionOnce changed the pos, so collision happened
			if (newPos != posTest)
			{
				pos = newPos;
				goto end;
			}
		} while (Engine::Vec2Length((newPos + delta) - pos) > GRANULARITY);

		// check last time
		checkCollisionOnce(mapData, pos);
	}

end:
	// don't get out of the world on x
	if (pos.x - transform.w / 2.f < 0) { pos.x = transform.w / 2.f; }
	if (pos.x + transform.w / 2.f > mapData.w) { pos.x = mapData.w - transform.w / 2.f; }
	if (pos.y + transform.h / 2.f > mapData.h) { pos.y = mapData.h - transform.h / 2.f; }

	if (leftTouch && velocity.x < 0) { velocity.x = 0; }
	if (rightTouch && velocity.x > 0) { velocity.x = 0; }
	if (upTouch && velocity.y < 0) { velocity.y = 0; }
	if (downTouch && velocity.y > 0) { velocity.y = 0; }
}

void PhysicalEntity::checkCollisionOnce(GameMap& mapData, Engine::Vec2& pos)
{
	Engine::Vec2 delta = pos - lastPosition;

	Engine::Vec2 newPos = performCollisionsOnOneAxis(mapData, { pos.x, lastPosition.y }, { delta.x, 0 });

	pos = performCollisionsOnOneAxis(mapData, { newPos.x, pos.y }, { 0, delta.y });
}

Engine::Vec2 PhysicalEntity::performCollisionsOnOneAxis(GameMap& mapData, Engine::Vec2 pos, Engine::Vec2 delta)
{
	if (delta.x == 0 && delta.y == 0) { return pos; }

	// get entity dimentions
	Engine::Vec2 dimentions = { transform.w, transform.h };

	// make sure box that we check is bigger than entity collider box
	int minX = floor(pos.x - dimentions.x / 2.f - 1);
	int maxX = ceil((pos.x + dimentions.x / 2.f + 1));
	int minY = floor(pos.y - dimentions.y / 2.f - 1);
	int maxY = ceil((pos.y + dimentions.y / 2.f + 1));

	// make sure box is not bigger or smaller than map
	minX = std::max(0, minX);
	minY = std::max(0, minY);
	maxX = std::min(maxX, mapData.w);
	maxY = std::min(maxY, mapData.h);
	
	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			bool isPlatform = mapData.getBlockUnsafe(x, y).isPlatform();
			bool isCollidable = mapData.getBlockUnsafe(x, y).isCollidable();

			if (isPlatform)
			{
				if (isIgnoringPlatforms())
					continue;

				if (delta.x != 0)
					continue;

				if (delta.y > 0) // moving downward
				{
					float oldBottom = lastPosition.y + transform.h / 2.f;
					float newBottom = pos.y + transform.h / 2.f;

					float platformTop = (float)y;

					float entityLeft = pos.x - transform.w / 2.f;
					float entityRight = pos.x + transform.w / 2.f;

					float blockLeft = (float)x;
					float blockRight = (float)x + 1.f;

					bool overlapsX = entityRight > blockLeft && entityLeft < blockRight;

					if (overlapsX && oldBottom <= platformTop && newBottom >= platformTop) // platform crossed
					{
						standingOnPlatform = true;
						downTouch = true;
						pos.y = platformTop - transform.h / 2.f;
						return pos;
					}
				}
				continue;
			}

			else if (isCollidable)
			{
				Transform2D entity;
				entity.pos = pos;
				entity.w = dimentions.x;
				entity.h = dimentions.y;

				Transform2D block;
				block.pos = { x + .5f, y + .5f };
				block.w = 1;
				block.h = 1;

				// really small delta makes player harder to intersact
				if (entity.intersectTransform(block, -0.00005f))
				{
					if (delta.x != 0)
					{
						if (delta.x < 0) // moving left
						{
							leftTouch = 1;
							pos.x = x + 1.f + dimentions.x / 2.f;
							return pos;
						}
						else // moving right
						{
							rightTouch = 1;
							pos.x = x - dimentions.x / 2.f;
							return pos;
						}
					}

					else if (delta.y != 0)
					{
						if (delta.y < 0) // moving up
						{
							upTouch = 1;
							pos.y = y + 1.f + dimentions.y / 2.f;
							return pos;
						}
						else // moving down
						{
							downTouch = 1;
							pos.y = y - dimentions.y / 2.f;
							return pos;
						}
					}
				}
			}
		}
	}

	return pos;
}