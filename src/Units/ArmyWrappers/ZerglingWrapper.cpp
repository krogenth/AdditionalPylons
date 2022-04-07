#include "./ZerglingWrapper.h"

#include <unordered_map>
#include <iterator>

#include <BWEB.h>

#include "../../Strategist/Strategist.h"
#include "../../Strategist/ScoutEngine/ScoutEngine.h"
#include "../../Players/Player.h"
#include "../../Lambdas/Map/MapLambdas.h"
#include "../../Lambdas/Unit/UnitLambdas.h"

void ZerglingWrapper::onFrame() {
	if (!this->unit->isCompleted()) return;

	std::unordered_map<int, BWAPI::Unit> units;
	std::vector<BWAPI::Unit> vectorUnits;
	std::set<const BWEM::Area*> areas;
	std::vector<const BWEM::Area*> vectorAreas;

	switch(Strategist::getInstance().getPlayDecision()) {
	case PlayDecision::scout:
		if (!this->scoutLocation.isValid()) {
			this->scoutLocation = ScoutEngine::getInstance().getNextBaseToScout();
		}
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
		break;

	case PlayDecision::attack:
		units = Player::getEnemyInstance().getUnitsByPredicate(lambdas::unit::getAllVisibleUnitsLambda);
		std::transform(units.begin(), units.end(), std::back_inserter(vectorUnits), [](auto& keyvalue) { return keyvalue.second; });
		std::sort(vectorUnits.begin(), vectorUnits.end(), [this](const BWAPI::Unit& a, const BWAPI::Unit& b) -> bool {
			auto aCanAttack = a->getType().canAttack(), bCanAttack = b->getType().canAttack();
			if (aCanAttack == bCanAttack) {
				return this->unit->getDistance(a) < this->unit->getDistance(b);
			} else {
				return aCanAttack > bCanAttack;
			}
		});
		
		if (vectorUnits.size()) {
			if (this->unit->getLastCommand().getTarget() != vectorUnits.at(0)) {
				if (this->unit->getTarget() != vectorUnits.at(0)) {
					this->unit->attack(vectorUnits.at(0));
				}
			}
		} else {
			areas = Player::getEnemyInstance().getBuildingAreas();
			std::transform(areas.begin(), areas.end(), std::back_inserter(vectorAreas), [](auto& keyvalue) { return keyvalue; });
			std::sort(vectorAreas.begin(), vectorAreas.end(), [this](const BWEM::Area* a, const BWEM::Area* b) -> bool {
				BWAPI::TilePosition aCenter = (a->TopLeft() + a->BottomRight()) / 2, bCenter = (b->TopLeft() + b->BottomRight());
				return this->unit->getDistance(BWAPI::Position(aCenter)) < this->unit->getDistance(BWAPI::Position(bCenter));
			});

			if (vectorAreas.size()) {
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
		break;
	case PlayDecision::defend:
		break;
	case PlayDecision::none:
		this->unit->move(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
		break;
	}
}

void ZerglingWrapper::displayInfo() {
	if (this->unit->getLastCommand().getTarget()) {
		BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), this->unit->getLastCommand().getTarget()->getPosition(), BWAPI::Colors::White);
	} else if (this->unit->getLastCommand().getTargetPosition().isValid()) {
		BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), this->unit->getLastCommand().getTargetPosition(), BWAPI::Colors::White);
	}
}