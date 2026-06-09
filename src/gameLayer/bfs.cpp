#include "bfs.h"
#include <queue>
#include "gameMap.h"


bool isValid(Vector2i pos, GameMap& gameMap)
{
	auto* b = gameMap.getBlockSafe(pos.x, pos.y);

	if (!b) return false;

	return true;
}

std::vector<Vector2i> bfs(Vector2i startPosition, GameMap& gameMap, int w, int h)
{
	std::queue<Vector2i> q;
	std::vector<Vector2i> component;
	std::vector<std::vector<bool>> visited(
		h,
		std::vector<bool>(w, false)
	);

	q.push(startPosition);
	visited.insert(startPosition);

	while (!q.empty())
	{
		Vector2i curr = q.front();
		q.pop();

		for (auto dir : directions)
		{
			Vector2i next = curr + dir;

			if (!isValid(next))
				continue;

			if (visited[next.y][next.x])
				continue;
		}
	}

	return std::vector<Vector2i>();
}
