#pragma once
#include "../UnitWrapper.h"

class ArmyWrapper : public UnitWrapper {
public:
	ArmyWrapper(BWAPI::Unit u) : UnitWrapper(u) {};
	~ArmyWrapper() = default;
protected:
	BWAPI::TilePosition scoutLocation = BWAPI::TilePositions::Invalid;
};