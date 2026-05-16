#include "tool.h"
#include <player.h>
#include <gameMap.h>

std::vector<ToolSwing> toolSwings;

void spawnToolSwing(Player* player, Vector2 mousePosition, float range, int power)
{
	ToolSwing swing{};
	swing.player = player;
	swing.mousePosition = mousePosition;
	swing.range = range;
	swing.power = power;

	swing.lifetime = .4f;
	swing.direction = !player->animations.movingLeft;

	//swing.position.x += direction * range;

	toolSwings.push_back(swing);
}

ToolHitResult updateToolSwing(float deltatime, GameMap& gameMap)
{
	ToolHitResult result;

	for (int i = (int)toolSwings.size() - 1; i >= 0; --i)
	{
		ToolSwing& swing = toolSwings[i];

		swing.lifetime -= deltatime;

		if (swing.lifetime <= 0)
		{
			swing.lifetime = 0;
			toolSwings.erase(toolSwings.begin() + i);
			continue;
		}

		auto* b = gameMap.getBlockSafe((int)swing.mousePosition.x, (int)swing.mousePosition.y);

		if (b == nullptr || b->type == b->air) return result;

		float dist = Vector2Distance(swing.mousePosition, swing.player->getPosition());

		if (dist <= swing.range)
		{
			// hit block
			b->hp -= swing.power;

			result.hit = true;
			result.type = b->type;
			result.position = Vector2{ swing.mousePosition.x, swing.mousePosition.y };
			result.power = swing.power;

			// block broken
			if (b->hp <= 0)
			{
				*b = {};
				result.broke = true;
			}

			swing.lifetime = 0;

			return result;
		}
	}

	return result;
}
