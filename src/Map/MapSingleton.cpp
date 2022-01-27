#include "MapSingleton.h"

void MapSingleton::onStart() {
	m_size = BWAPI::TilePosition(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
}