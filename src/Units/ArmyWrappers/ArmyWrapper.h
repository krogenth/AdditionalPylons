#pragma once
#include "../UnitWrapper.h"

class ArmyWrapper : public UnitWrapper {
public:
	ArmyWrapper(BWAPI::Unit u) : UnitWrapper(u) {};
};