#pragma once

#include <BWAPI.h>
#include <bwem.h>
#include <BWEB.h>

#include "../../Players/Player.h"

namespace lambdas::unit {
	const std::function<bool (const std::shared_ptr<UnitWrapper>)> getAllUnitsLambda = [](const std::shared_ptr<UnitWrapper> unit) -> bool {
		return unit.get()->getUnitType() != BWAPI::UnitTypes::Unknown;
	};

	const std::function<bool (const std::shared_ptr<UnitWrapper>)> getAllVisibleUnitsLambda = [](const std::shared_ptr<UnitWrapper> unit) -> bool {
		return unit.get()->getUnit()->isVisible(BWAPI::Broodwar->self());
	};

	const std::function<bool(const std::shared_ptr<UnitWrapper>)> getResourceDepotUnitsLambda = [](const std::shared_ptr<UnitWrapper> unit) -> bool {
		return unit.get()->getUnitType() == Player::getEnemyInstance().getRace().getResourceDepot();
	};

	const std::function<bool(const std::shared_ptr<UnitWrapper>)> getBuildingsLambda = [](const std::shared_ptr<UnitWrapper> unit) -> bool {
		return unit.get()->getUnitType().isBuilding();
	};

	const std::function<bool(const std::shared_ptr<UnitWrapper>)> getArmyUnitsLambda = [](const std::shared_ptr<UnitWrapper> unit) -> bool {
		return unit.get()->getUnitType().canAttack();
	};

	const std::function<bool(const std::shared_ptr<UnitWrapper>)> getVisibleArmyUnitsLambda = [](const std::shared_ptr<UnitWrapper> unit) -> bool {
		return unit.get()->getUnitType().canAttack() && unit.get()->getUnit()->isVisible(BWAPI::Broodwar->self());
	};
}