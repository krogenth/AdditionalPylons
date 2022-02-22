#pragma once
#include "../UnitWrapper.h"

class NonArmyWrapper : public UnitWrapper {
public:
	NonArmyWrapper(BWAPI::Unit u) : UnitWrapper(u) {};
};