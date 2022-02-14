#pragma once
#include "BWAPI.h"
#include "UnitWrapper.h"

class ArmyWrapper: public UnitWrapper
{
protected:
    BWAPI::Unit curr_target = nullptr;
public:
    ArmyWrapper(BWAPI::Unit u):UnitWrapper(u){};
};