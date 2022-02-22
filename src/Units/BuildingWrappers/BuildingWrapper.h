#pragma once
#include "../UnitWrapper.h"

class BuildingWrapper : public UnitWrapper {
public:
    BuildingWrapper(BWAPI::Unit u) : UnitWrapper(u) {};

    virtual void onFrame();
    virtual void displayInfo();
};