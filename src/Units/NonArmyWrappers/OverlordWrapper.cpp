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
		if (!this->scoutLocation.isValid()) {
			this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
		}
		if (!BWAPI::Broodwar->isVisible(this->scoutLocation) && this->unit->getTargetPosition() != BWAPI::Position(this->scoutLocation)) {
			this->unit->move(BWAPI::Position(this->scoutLocation));
		} else if (BWAPI::Broodwar->isVisible(this->scoutLocation)) {
			// only reset the scout's location if they aren't an enemy base
			if (enemyAreas.empty() || (enemyAreas.find(BWEM::Map::Instance().GetArea(this->scoutLocation)) == enemyAreas.end())) {
				this->scoutLocation = BWAPI::TilePositions::Invalid;
			}
		}
		break;
	case PlayDecision::attack:
		[[fallthrough]];
	case PlayDecision::defend:
		[[fallthrough]];
	case PlayDecision::none:
		// check if the unit is currently going home, if not return home
		if (this->unit->getLastCommand().getTargetPosition() != BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation())) {
			this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
			this->scoutLocation = BWAPI::TilePositions::Invalid;
		}
		break;
	}
}

void OverlordWrapper::displayInfo() {
    if (this->unit->getLastCommand().getTarget()) {
        BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), this->unit->getLastCommand().getTarget()->getPosition(), BWAPI::Colors::White);
    }
    else if (this->unit->getLastCommand().getTargetPosition().isValid()) {
        BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), this->unit->getLastCommand().getTargetPosition(), BWAPI::Colors::White);
    }
    else if (this->unit->getLastCommand().getTargetTilePosition().isValid()) {
        BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), BWAPI::Position(this->unit->getLastCommand().getTargetTilePosition()), BWAPI::Colors::White);
    }
}