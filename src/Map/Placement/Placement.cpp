#include "Placement.h"

#include <set>
#include <queue>

namespace Placement {

	bool isValidPosition(const BWAPI::UnitType& type, const BWAPI::TilePosition& pos, const BWAPI::TilePosition& mapSize) {
		if (pos.x < 0 || pos.x > mapSize.x || pos.y < 0 || pos.y > mapSize.y)
			return false;
		return BWAPI::Broodwar->canBuildHere(pos, type);
	}

	BWAPI::TilePosition getOpenPlacement(const BWAPI::UnitType& type) {
		BWAPI::TilePosition start = BWAPI::Broodwar->self()->getStartLocation();
		BWAPI::TilePosition mapSize = BWAPI::TilePosition(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());

		std::set<BWAPI::TilePosition> visited;
		std::queue<BWAPI::TilePosition> posQueue({start});
		while (!posQueue.empty()) {
			auto pos = posQueue.front();
			visited.emplace(pos);
			posQueue.pop();

			if (isValidPosition(type, pos, mapSize))
				return pos;

			if ((pos.x > 1) && (visited.find(BWAPI::TilePosition(pos.x - 1, pos.y)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x - 1, pos.y));
			if ((pos.x < (mapSize.x - 1)) && (visited.find(BWAPI::TilePosition(pos.x + 1, pos.y)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x + 1, pos.y));
			if ((pos.y > 1) && (visited.find(BWAPI::TilePosition(pos.x, pos.y - 1)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x, pos.y - 1));
			if ((pos.y < (mapSize.y - 1)) && (visited.find(BWAPI::TilePosition(pos.x, pos.y + 1)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x, pos.y + 1));
		}

		return BWAPI::TilePositions::Invalid;
	}

}