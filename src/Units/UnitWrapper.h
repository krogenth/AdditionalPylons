#pragma once
#include "BWAPI.h"
#include <queue>

class UnitWrapper {
public:
    UnitWrapper(BWAPI::Unit u) : unit(u), unitID(u->getID()), type(u->getType()) {}
    ~UnitWrapper() = default;

    BWAPI::Unit getUnit() { return this->unit; }
    BWAPI::UnitType getUnitType() { return this->type; }
    int getID() { return this->unitID; }

    virtual const BWAPI::Position& getPosition() { return this->pos; }
    virtual const BWAPI::TilePosition& getTilePosition() { return this->tilePos; }
    virtual void updatePosition() {
        if (this->unit->isVisible(BWAPI::Broodwar->self()) && this->unit->getPosition().isValid()) {
            this->pos = this->unit->getPosition();
            this->tilePos = this->unit->getTilePosition();
        }
    }

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
    virtual void displayInfo() {}
    

protected:
    virtual void handleScoutStrategy() {};
	virtual void handleAttackStrategy() {};
	virtual void handleDefenseStrategy() {};

    BWAPI::Position pos = BWAPI::Positions::Invalid;
    BWAPI::TilePosition tilePos = BWAPI::TilePositions::Invalid;
    std::queue<BWAPI::TilePosition> queue;
    int unitID;
    BWAPI::Unit unit;
    BWAPI::UnitType type;
    BWAPI::UnitType buildOrder = BWAPI::UnitTypes::Unknown;
};