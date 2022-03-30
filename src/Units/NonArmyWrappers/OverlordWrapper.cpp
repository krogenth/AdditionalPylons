#include "./OverlordWrapper.h"

#include <bwem.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"

void OverlordWrapper::onFrame() {
	if (!this->unit->isCompleted()) return;

	auto enemyAreas = Player::getEnemyInstance().getBuildingAreas();

	switch(Strategist::getInstance().getPlayDecision()) {
	case PlayDecision::scout:
		if (this->scoutLocation == BWAPI::TilePositions::Invalid) {
			this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
		}
		if (!BWAPI::Broodwar->isVisible(this->scoutLocation) && this->unit->getLastCommand().getTargetTilePosition() != this->scoutLocation) {
			this->unit->move(BWAPI::Position(this->scoutLocation), true);
		} else if (BWAPI::Broodwar->isVisible(this->scoutLocation)) {
			if (this->scoutLocation.isValid()) {
				// only reset the scout's location if they aren't at the enemy location
				if (enemyAreas.find(BWEM::Map::Instance().GetArea(this->scoutLocation)) == enemyAreas.end()) {
					this->scoutLocation == BWAPI::TilePositions::Invalid;
				}
			}
		}
		break;
	case PlayDecision::attack:
		[[fallthrough]];
	case PlayDecision::defend:
		[[fallthrough]];
	case PlayDecision::none:
		// check where the unit is currently, if it's not at the enemy base, return home
		if (enemyAreas.find(BWEM::Map::Instance().GetArea(this->unit->getTilePosition())) == enemyAreas.end()) {
			if (this->unit->getTilePosition() != BWAPI::Broodwar->self()->getStartLocation()) {
				this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
			}
		}
		break;
	}
}

void OverlordWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "OverlordWrapper");
}