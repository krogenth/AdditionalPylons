#pragma once
#include "UnitWrapper.h"

class BuildingWrapper : public UnitWrapper
{
protected:
public:
    BuildingWrapper(BWAPI::Unit u) : UnitWrapper(u){};
    bool isBusy()
    {
        return unit->isConstructing() || unit->isMorphing();
    }

    void onFrame() {}

    void displayInfo()
    {
        if (isBusy())
        {
            BWAPI::Broodwar->drawTextMap(unit->getPosition(), "busy");
        }
        else
        {
            BWAPI::Broodwar->drawTextMap(unit->getPosition(), "idle");
        }
    }
};