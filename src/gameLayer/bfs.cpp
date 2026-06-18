#include "bfs.h"
#include <queue>
#include "gameMap.h"
#include "items/item.h"


Vector2i directions[3] =
{
	{ 1, 0 },
	{-1, 0 },
	//{ 0, 1 },
	{ 0,-1 }
};

bool isValidTree(Vector2i pos, GameMap& gameMap)
{
	auto* b = gameMap.getBlockSafe(pos.x, pos.y);

	if (!b) return false;
	if (b->type != Items::woodLog && b->type != Items::leaves) return false;

	return true;
}

std::vector<Vector2i> bfs(Vector2i startPosition, GameMap& gameMap)
{
	printf("bfs called...\n");
	std::queue<Vector2i> q;
	std::vector<Vector2i> component;
	std::unordered_set<Vector2i, Vector2iHash> visited;

	q.push(startPosition);
	//component.push_back(startPosition); // its already broken, so no need to include it
	visited.insert(startPosition);

	while (!q.empty())
	{
		Vector2i curr = q.front();
		q.pop();

		for (auto dir : directions)
		{
			Vector2i next = curr + dir;

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
