#pragma once
#include "UnitWrapper.h"

enum class Jobs
{
    MineMinerals,
    ExtractGas,
    MorphIntoNewBuildings,
    Scout,
    None
};

class WorkerWrapper : public UnitWrapper
{
protected:
    Jobs CurrJob = Jobs::None;

public:
    WorkerWrapper(BWAPI::Unit u) : UnitWrapper(u){};
    bool isBusy()
    {
        if (CurrJob == Jobs::None)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void onFrame()
    {
        BWAPI::Unit closest = nullptr;
        int smallestDistance = INT32_MAX;
        if (CurrJob == Jobs::None)
        {
            for (const auto &i : BWAPI::Broodwar->getMinerals())
            {
                if (unit->getDistance(i) < smallestDistance)
                {
                    smallestDistance = unit->getDistance(i);
                    closest = i;
                }
            }
            if (closest)
            {
                unit->gather(closest);
                currJob = Jobs::MineMinerals;
            }
        }
    }
};