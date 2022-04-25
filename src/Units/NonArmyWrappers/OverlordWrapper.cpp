#include "./OverlordWrapper.h"

#include <bwem.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"
#include "../../Lambdas/Map/MapLambdas.h"

void OverlordWrapper::onFrame() {
	if (!this->unit->isCompleted()) return;

	auto enemyAreas = Player::getEnemyInstance().getBuildingAreas();

	switch(Strategist::getInstance().getPlayDecision()) {
	case PlayDecision::scout: this->handleScoutStrategy(); break;
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

void OverlordWrapper::handleScoutStrategy() {
	// check if we currently have a base to scout
	if (!this->scoutLocation.isValid()) {
		this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
	}

	// if we aren't at the scout location and we did not issue an order to go that location, issue the command
	if (!BWAPI::Broodwar->isVisible(this->scoutLocation) && this->unit->getTargetPosition() != BWAPI::Position(this->scoutLocation)) {
		this->unit->move(BWAPI::Position(this->scoutLocation), true);
	} else if (BWAPI::Broodwar->isVisible(this->scoutLocation)) {
		if (this->scoutLocation.isValid()) {
			if (Player::getEnemyInstance().getBuildingAreas().empty() ||
				Player::getEnemyInstance().getBuildingAreas().find(BWEM::Map::Instance().GetArea(this->scoutLocation)) ==
					Player::getEnemyInstance().getBuildingAreas().end()) {
				this->scoutLocation = BWAPI::TilePositions::Invalid;
			}
		}
	}
}