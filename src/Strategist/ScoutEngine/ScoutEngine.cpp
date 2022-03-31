#include "./ScoutEngine.h"

#include <algorithm>

#include <bwem.h>

void ScoutEngine::onStart() {
	for (const auto& area : BWEM::Map::Instance().Areas()) {
		for (const auto& base : area.Bases()) {
			this->baseLocationFrameVisitibilityMap.emplace_back(std::make_pair(base.Center(), BWAPI::Broodwar->getFrameCount()));
		}
	}
	
	this->sortScoutBases();
}

void ScoutEngine::onFrame() {
	for (auto& location : this->baseLocationFrameVisitibilityMap) {
		if (BWAPI::Broodwar->isVisible(location.first)) {
			location.second = BWAPI::Broodwar->getFrameCount();
		}
	}
}

BWAPI::TilePosition ScoutEngine::getNextBaseToScout() {
	// grab location first before sorting
	auto location = this->baseLocationFrameVisitibilityMap.begin()->first;

	// swap first location with last before sorting, to ensure selected location is not first next
	std::swap(this->baseLocationFrameVisitibilityMap.front(), this->baseLocationFrameVisitibilityMap.back());
	
	// re-sort bases
	this->sortScoutBases();

	return location;
}

void ScoutEngine::displayInfo() {
	for (const auto& location : this->baseLocationFrameVisitibilityMap) {
		BWAPI::Broodwar->drawTextMap(BWAPI::Position(location.first), "frame: %d", location.second);
	}
}

void ScoutEngine::sortScoutBases() {
	std::sort(this->baseLocationFrameVisitibilityMap.begin(), this->baseLocationFrameVisitibilityMap.end(),
		[](const std::pair<BWAPI::TilePosition, int>& a, const std::pair<BWAPI::TilePosition, int>& b) {
			auto exploredA = BWAPI::Broodwar->isExplored(a.first);
			auto exploredB = BWAPI::Broodwar->isExplored(b.first);
			auto startingLocations = BWAPI::Broodwar->getStartLocations();
			auto isAStarting = std::find(startingLocations.begin(), startingLocations.end(), a.first) != startingLocations.end();
			auto isBStarting = std::find(startingLocations.begin(), startingLocations.end(), b.first) != startingLocations.end();
			if (isAStarting == isBStarting) {
				if (exploredA == exploredB) {
					return a.second < b.second;
				} else {
					return exploredA < exploredB;
				}
			} else {
				return isAStarting > isBStarting;
			}
		});
}