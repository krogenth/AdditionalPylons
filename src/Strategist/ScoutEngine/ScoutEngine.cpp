#include "./ScoutEngine.h"

#include <algorithm>

void ScoutEngine::onStart() {
	this->startingLocations = BWAPI::Broodwar->getStartLocations();
}

void ScoutEngine::onFrame() {
	// not sure what needs to be done here yet, if anything
}

void ScoutEngine::setScout(BWAPI::Unit unit) {
	this->scout = unit;
}

void ScoutEngine::onUnitDestroy(BWAPI::Unit unit) {
	if (unit == this->scout)
		this->scout = nullptr;
}

BWAPI::TilePosition ScoutEngine::getNextBaseToScout() {
	//	first, sort the starting bases
	std::sort(this->startingLocations.begin(), this->startingLocations.end(),
		[](const BWAPI::TilePosition& a, const BWAPI::TilePosition& b) {
			return BWAPI::Broodwar->isExplored(a) < BWAPI::Broodwar->isExplored(b);
		});
	return this->startingLocations.front();
}