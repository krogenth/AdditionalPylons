#include "./LurkerWrapper.h"

#include <bwem.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"
#include "../../Lambdas/Map/MapLambdas.h"

void LurkerWrapper::onFrame() {
    if (!this->unit->isCompleted()) return;

	switch(Strategist::getInstance().getPlayDecision()) {
	case PlayDecision::scout: this->handleScoutStrategy(); break;
	case PlayDecision::attack: this->handleAttackStrategy(); break;
	case PlayDecision::defend: this->handleDefenseStrategy(); break;
	case PlayDecision::none: this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation())); break;
	}
}

void LurkerWrapper::displayInfo() {
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

void LurkerWrapper::handleScoutStrategy() {
	// check if we currently have a base to scout
	if (!this->scoutLocation.isValid()) {
		this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
	}

	// if we aren't at the scout location and we did not issue an order to go that location, issue the command
	if (this->scoutLocation != this->unit->getTilePosition() && this->unit->getTargetPosition() != BWAPI::Position(this->scoutLocation)) {
		this->unit->move(BWAPI::Position(this->scoutLocation));
	} else if (this->scoutLocation == this->unit->getTilePosition()) {
		if (this->scoutLocation.isValid()) {
			if (Player::getEnemyInstance().getBuildingAreas().empty() ||
				Player::getEnemyInstance().getBuildingAreas().find(BWEM::Map::Instance().GetArea(this->scoutLocation)) ==
					Player::getEnemyInstance().getBuildingAreas().end()) {
				this->scoutLocation = BWAPI::TilePositions::Invalid;
			}
		}
	}
}

void LurkerWrapper::handleAttackStrategy() {

}

void LurkerWrapper::handleDefenseStrategy() {
	
}