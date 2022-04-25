#pragma once
#include "../UnitWrapper.h"

class ArmyWrapper : public UnitWrapper {
public:
	ArmyWrapper(BWAPI::Unit u) : UnitWrapper(u) {};
	~ArmyWrapper() = default;

protected:
	std::vector<std::shared_ptr<UnitWrapper>> getListOfEnemyUnitsByPredicate(std::function <bool(const std::shared_ptr<UnitWrapper>)> filterPredicate);

	BWAPI::TilePosition scoutLocation = BWAPI::TilePositions::Invalid;
};