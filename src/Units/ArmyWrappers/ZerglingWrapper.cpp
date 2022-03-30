#include "./ZerglingWrapper.h"

#include <unordered_map>
#include <BWEB.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"

void ZerglingWrapper::onFrame() {
	if (!this->unit->isCompleted()) return;

	auto enemyAreas = Player::getEnemyInstance().getBuildingAreas();
	BWEB::Path path = BWEB::Path();
	size_t shortestPath = SIZE_MAX;
	BWAPI::Unit closestUnit = nullptr;
	const BWEM::Area* closestArea = nullptr;

	switch(Strategist::getInstance().getPlayDecision()) {
	case PlayDecision::scout:
		if (this->scoutLocation == BWAPI::TilePositions::Invalid) {
			this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
		}
		if (!BWAPI::Broodwar->isVisible(this->scoutLocation) && this->unit->getLastCommand().getTargetTilePosition() != this->scoutLocation) {
			this->unit->move(BWAPI::Position(this->scoutLocation), true);
		} else if (BWAPI::Broodwar->isVisible(this->scoutLocation)) {
			if (this->scoutLocation.isValid()) {
				if (enemyAreas.size() <= 0 || enemyAreas.find(BWEM::Map::Instance().GetArea(this->scoutLocation)) == enemyAreas.end()) {
					this->scoutLocation == BWAPI::TilePositions::Invalid;
				}
			}
		}
		break;
	case PlayDecision::attack:
		closestUnit = Player::getEnemyInstance().getClosestUnitTo(this->unit->getPosition(), this->type);
		if (closestUnit) {
			if (this->unit->getLastCommand().getTarget() != closestUnit) {
				if (closestUnit->isVisible(BWAPI::Broodwar->self())) {
					this->unit->attack(closestUnit);
				} else {
					this->unit->move(closestUnit->getPosition());
				}
			}
		} else {
			closestArea = Player::getEnemyInstance().getClosestAreaTo(this->unit->getPosition(), this->type);
			if (closestArea) {
				BWAPI::TilePosition areaCenter = (closestArea->TopLeft() + closestArea->BottomRight()) / 2;
				if (this->unit->getLastCommand().getTargetTilePosition() != areaCenter) {
					this->unit->move(BWAPI::Position(areaCenter));
				}
			}
		}
		break;
	case PlayDecision::defend:
		break;
	case PlayDecision::none:
		this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
		break;
	}
}

void ZerglingWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "ZerglingWrapper");
}