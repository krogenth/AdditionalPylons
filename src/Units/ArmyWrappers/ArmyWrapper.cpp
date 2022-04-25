#include "./ArmyWrapper.h"

#include "../../Players/Player.h"

std::vector<std::shared_ptr<UnitWrapper>> ArmyWrapper::getListOfEnemyUnitsByPredicate(std::function <bool(const std::shared_ptr<UnitWrapper>)> filterPredicate) {
    std::vector<std::shared_ptr<UnitWrapper>> vectorUnits;
    auto units = Player::getEnemyInstance().getUnitsByPredicate(filterPredicate);
	std::transform(units.begin(), units.end(), std::back_inserter(vectorUnits), [](auto& keyvalue) { return keyvalue.second; });
    return vectorUnits;
}