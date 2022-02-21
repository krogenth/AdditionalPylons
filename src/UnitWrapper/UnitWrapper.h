#pragma once
#include "BWAPI.h"
#include <queue>

class UnitWrapper
{
protected:
    std::queue<BWAPI::TilePosition> queue;
    int unitID;
    BWAPI::Unit unit;
public:
    UnitWrapper(BWAPI::Unit u)
    {
        unit = u;
        unitID = u->getID();
    };

    virtual bool isBusy() = 0;
    virtual void OnFrame() = 0;

    void Draw()
    {
        if (!queue.empty())
        { //team fortress 2 is the best game ever no cap
            BWAPI::Broodwar->drawLineMap(BWAPI::Position(unit->getTilePosition()), BWAPI::Position(queue.back()), BWAPI::Colors::White);
        }
    };
};