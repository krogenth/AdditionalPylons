#include "./OverlordWrapper.h"

#include <bwem.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"

void OverlordWrapper::onFrame() {
	auto play = Strategist::getInstance().getPlayDecision();
	if (play == PlayDecision::scout) {
		if (this->scoutLocation == BWAPI::TilePositions::Invalid) {
			this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
		}
		if (!BWAPI::Broodwar->isVisible(this->scoutLocation) && this->unit->getLastCommand().getTargetTilePosition() != this->scoutLocation) {
			this->unit->move(BWAPI::Position(this->scoutLocation));
		} else if (BWAPI::Broodwar->isVisible(this->scoutLocation)) {
			auto area = BWEM::Map::Instance().GetArea(this->scoutLocation);
			auto enemyAreas = Player::getEnemyInstance().getBuildingAreas();
			// only reset the scout's location if they aren't at the enemy location
			if (enemyAreas.find(area) == enemyAreas.end()) {
				this->scoutLocation == BWAPI::TilePositions::Invalid;
			}
		}
	} else {
		auto area = BWEM::Map::Instance().GetArea(this->unit->getTilePosition());
		auto enemyAreas = Player::getEnemyInstance().getBuildingAreas();
		if (enemyAreas.find(area) == enemyAreas.end()) {
			this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
		}
	}
}

void OverlordWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "OverlordWrapper");
}