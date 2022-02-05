#pragma once
#include "BWAPI.h"
#include <queue>

class UnitWrapper
{
    std::queue<BWAPI::TilePosition> queue;
    int unitID;
    BWAPI::Unit unit;

    UnitWrapper(BWAPI::Unit u)
    {
        unit = u;
        unitID = u->getID();
    };

    virtual bool isBusy() = 0;
    virtual void Order() = 0;

    void Draw()
    {
        if (!queue.empty())
        {
            BWAPI::Broodwar->drawLineMap(unit->getTilePosition(), queue.back(), BWAPI::Colors::White);
        }
    };
};