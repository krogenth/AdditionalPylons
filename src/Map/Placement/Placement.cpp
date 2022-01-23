#include "Placement.h"

#include <set>
#include <queue>

namespace Placement {

	bool isValidPosition(const BWAPI::UnitType& type, const BWAPI::TilePosition& pos, const BWAPI::TilePosition& mapSize) {
		//	check if position it outside bounds of map
		if (pos.x < 0 || pos.x > mapSize.x || pos.y < 0 || pos.y > mapSize.y)
			return false;
		//	check that it is possible to build at this location
		return BWAPI::Broodwar->canBuildHere(pos, type);
	}

	BWAPI::TilePosition getOpenPlacement(const BWAPI::UnitType& type) {
		//	grab require information, Player starting location and map size
		BWAPI::TilePosition start = BWAPI::Broodwar->self()->getStartLocation();
		BWAPI::TilePosition mapSize = BWAPI::TilePosition(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());

		//	start visited list and depth queue
		std::set<BWAPI::TilePosition> visited;
		std::queue<BWAPI::TilePosition> posQueue({start});

		//	iterate depth queue until a position is found or no more positions exist
		while (!posQueue.empty()) {
			auto pos = posQueue.front();
			visited.emplace(pos);
			posQueue.pop();

			if (visited.find(pos) == visited.end() && isValidPosition(type, pos, mapSize))
				return pos;

			if (((pos.x - 1) >= 0) && (visited.find(BWAPI::TilePosition(pos.x - 1, pos.y)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x - 1, pos.y));
			if ((pos.x <= mapSize.x) && (visited.find(BWAPI::TilePosition(pos.x + 1, pos.y)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x + 1, pos.y));
			if (((pos.y - 1) >= 0) && (visited.find(BWAPI::TilePosition(pos.x, pos.y - 1)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x, pos.y - 1));
			if ((pos.y <= mapSize.y) && (visited.find(BWAPI::TilePosition(pos.x, pos.y + 1)) != visited.end()))
				posQueue.push(BWAPI::TilePosition(pos.x, pos.y + 1));
		}

		return BWAPI::TilePositions::Invalid;
	}

}