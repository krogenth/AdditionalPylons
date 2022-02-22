#pragma once
#include "BWAPI.h"
#include <queue>

class UnitWrapper {
public:
    UnitWrapper(BWAPI::Unit u) : unit(u), unitID(u->getID()), type(u->getType()) {}

    BWAPI::Unit getUnit() { return this->unit; }
    BWAPI::UnitType getUnitType() { return this->type; }

    virtual bool isBusy() {
        if (!this->unit->isCompleted() ||
            this->unit->isCarryingMinerals() ||
            this->unit->isCarryingGas() ||
            this->unit->isMorphing() ||
            this->unit->isConstructing())
            return true;
        return false;
    }
    virtual void OnFrame() {}
    virtual void displayInfo() {}

protected:
    std::queue<BWAPI::TilePosition> queue;
    int unitID;
    BWAPI::Unit unit;
    BWAPI::UnitType type;
};