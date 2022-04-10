#include "./ZerglingWrapper.h"

#include <unordered_map>
#include <iterator>
#include <limits>

#include <bwem.h>
#include <BWEB.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"
#include "../../Lambdas/Map/MapLambdas.h"
#include "../../Lambdas/Unit/UnitLambdas.h"

void ZerglingWrapper::onFrame() {
	if (!this->unit->isCompleted()) return;

	switch(Strategist::getInstance().getPlayDecision()) {
	case PlayDecision::scout: this->handleScoutStrategy(); break;
	case PlayDecision::attack: this->handleAttackStrategy(); break;
	case PlayDecision::defend: this->handleDefenseStrategy(); break;
	case PlayDecision::none: this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation())); break;
	}
}

void ZerglingWrapper::displayInfo() {
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

void ZerglingWrapper::handleScoutStrategy() {
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

void ZerglingWrapper::handleAttackStrategy() {
	std::vector<std::shared_ptr<UnitWrapper>> vectorUnits;
	std::set<const BWEM::Area*> areas;
	std::vector<const BWEM::Area*> vectorAreas;

	// grab all the visible units owned by the enemy, convert to a vector of BWAPI::Unit, and sort by priority
	vectorUnits = this->getListOfEnemyUnitsByPredicate(lambdas::unit::getAllVisibleUnitsLambda);
	std::sort(vectorUnits.begin(), vectorUnits.end(), [this](const std::shared_ptr<UnitWrapper>& a, const std::shared_ptr<UnitWrapper>& b) -> bool {
		auto aCanAttack = a->getUnitType().canAttack(), bCanAttack = b->getUnitType().canAttack();
		if (aCanAttack == bCanAttack) {
			return this->unit->getDistance(a->getPosition()) < this->unit->getDistance(b->getPosition());
		}
		else {
			return aCanAttack > bCanAttack;
		}
	});
	
	// check if we have units to go to, if not, grab the enemy areas
	if (!vectorUnits.empty()) {
		if (this->unit->getLastCommand().getTarget() != vectorUnits.front()->getUnit()) {
			if (this->unit->getTarget() != vectorUnits.front()->getUnit()) {
				this->unit->attack(vectorUnits.front()->getUnit());
			}
		}
	} else {
		// check if there are any buildings associated with the enemy, sort by priority
		vectorUnits = this->getListOfEnemyUnitsByPredicate(lambdas::unit::getBuildingsLambda);
		std::sort(vectorUnits.begin(), vectorUnits.end(), [this](const std::shared_ptr<UnitWrapper>& a, const std::shared_ptr<UnitWrapper>& b) -> bool {
				return this->unit->getDistance(a->getPosition()) < this->unit->getDistance(b->getPosition());
		});

		if (!vectorUnits.empty()) {
			this->unit->move(vectorUnits.front()->getPosition());
		} else {
			// grab enemy areas, convert to a vector, and sort by distance
			areas = Player::getEnemyInstance().getBuildingAreas();
			std::transform(areas.begin(), areas.end(), std::back_inserter(vectorAreas), [](auto& keyvalue) { return keyvalue; });
			std::sort(vectorAreas.begin(), vectorAreas.end(), [this](const BWEM::Area* a, const BWEM::Area* b) -> bool {
				BWAPI::TilePosition aCenter = (a->TopLeft() + a->BottomRight()) / 2, bCenter = (b->TopLeft() + b->BottomRight());
				return this->unit->getDistance(BWAPI::Position(aCenter)) < this->unit->getDistance(BWAPI::Position(bCenter));
			});

			// if the area vector isn't empty, we have a base to go to
			if (!vectorAreas.empty()) {
				bool foundUnexploredBase = false;
				for (const auto& area : vectorAreas) {
					for (const auto& base : area->Bases()) {
						if (!BWAPI::Broodwar->isVisible(BWAPI::TilePosition(base.Center()))) {
							if (this->unit->getTargetPosition() != base.Center()) {
								this->unit->move(base.Center());
							}
							foundUnexploredBase = true;
							break;
						}
					}
					if (foundUnexploredBase) {
						break;
					}
				}
			}
		}
	}
}

void ZerglingWrapper::handleDefenseStrategy() {

}