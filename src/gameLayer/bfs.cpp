#include "bfs.h"
#include <queue>
#include "gameMap.h"
#include "items/item.h"


Engine::Vec2i directions[3] =
{
	{ 1, 0 },
	{-1, 0 },
	//{ 0, 1 },
	{ 0,-1 }
};

bool isValidTree(Engine::Vec2i pos, GameMap& gameMap)
{
	auto* b = gameMap.getBlockSafe(pos.x, pos.y);

	if (!b) return false;
	if (b->type != Items::woodLog && b->type != Items::leaves) return false;

	return true;
}

std::vector<Engine::Vec2i> bfs(Engine::Vec2i startPosition, GameMap& gameMap)
{
	std::queue<Engine::Vec2i> q;
	std::vector<Engine::Vec2i> component;
	std::unordered_set<Engine::Vec2i, Engine::Vec2iHash> visited;

	q.push(startPosition);
	//component.push_back(startPosition); // its already broken, so no need to include it
	visited.insert(startPosition);

	while (!q.empty())
	{
		Engine::Vec2i curr = q.front();
		q.pop();

		for (auto dir : directions)
		{
			Engine::Vec2i next = curr + dir;

			if (!isValidTree(next, gameMap))
				continue;

			if (visited.find(next) != visited.end())
				continue;

			component.push_back(next);
			q.push(next);
			visited.insert(next);
		}
	}

	return component;
}
