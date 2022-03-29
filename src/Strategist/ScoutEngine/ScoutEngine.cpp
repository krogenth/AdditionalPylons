#include "./ScoutEngine.h"

#include <algorithm>

void ScoutEngine::onStart() {
	auto locations = BWAPI::Broodwar->getStartLocations();
	for (const auto& location : locations) {
		this->startLocationFrameVisitibilityMap.emplace_back(std::make_pair(location, BWAPI::Broodwar->getFrameCount()));
	}
}

void ScoutEngine::onFrame() {
	for (auto& location : this->startLocationFrameVisitibilityMap) {
		if (BWAPI::Broodwar->isVisible(location.first)) {
			location.second = BWAPI::Broodwar->getFrameCount();
		}
	}
}

BWAPI::TilePosition ScoutEngine::getNextBaseToScout() {
	// grab location first before sorting
	auto location = this->startLocationFrameVisitibilityMap.begin()->first;

	// swap first location with last before sorting, to ensure selected location is not first next
	std::swap(this->startLocationFrameVisitibilityMap.front(), this->startLocationFrameVisitibilityMap.back());
	
	//	first, sort the starting bases
	std::sort(this->startLocationFrameVisitibilityMap.begin(), this->startLocationFrameVisitibilityMap.end(),
		[](const std::pair<BWAPI::TilePosition, int>& a, const std::pair<BWAPI::TilePosition, int>& b) {
			auto exploredA = BWAPI::Broodwar->isExplored(a.first);
			auto exploredB = BWAPI::Broodwar->isExplored(b.first);
			if (exploredA == exploredB) {
				return a.second < b.second;
			} else {
				return exploredA < exploredB;
			}
		});

	return location;
}

void ScoutEngine::displayInfo() {
	for (const auto& location : this->startLocationFrameVisitibilityMap) {
		BWAPI::Broodwar->drawTextMap(BWAPI::Position(location.first), "frame: %d", location.second);
	}
}