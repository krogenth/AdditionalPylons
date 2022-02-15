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
    Jobs currJob = Jobs::None;

public:
    WorkerWrapper(BWAPI::Unit u) : UnitWrapper(u){};
    bool isBusy()
    {
        if (currJob == Jobs::None)
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
        if (currJob == Jobs::None)
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