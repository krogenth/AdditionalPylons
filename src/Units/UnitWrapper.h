#pragma once
#include "BWAPI.h"
#include <queue>

class UnitWrapper {
public:
    UnitWrapper(BWAPI::Unit u) : unit(u), unitID(u->getID()), type(u->getType()) {}
    ~UnitWrapper() = default;

    BWAPI::Unit getUnit() { return this->unit; }
    BWAPI::UnitType getUnitType() { return this->type; }

    virtual bool isBusy() {
        if (!this->unit->isCompleted() ||
            this->unit->isCarryingMinerals() ||
            this->unit->isCarryingGas() ||
            this->unit->isMorphing() ||
            this->unit->isConstructing() ||
            this->unit->isUpgrading())
            return true;
        return false;
    }
    virtual void onFrame() {}
    virtual void displayInfo() { BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "UnitWrapper"); }

protected:
    std::queue<BWAPI::TilePosition> queue;
    int unitID;
    BWAPI::Unit unit;
    BWAPI::UnitType type;
    BWAPI::UnitType buildOrder = BWAPI::UnitTypes::Unknown;
};