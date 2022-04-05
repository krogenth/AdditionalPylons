#pragma once
#include "./NonArmyWrapper.h"
#include "bwem.h"

enum class Jobs
{
    MineMinerals,
    ExtractGas,
    MorphIntoNewBuildings,
    Scout,
    None
};

class WorkerWrapper : public NonArmyWrapper {
public:
    WorkerWrapper(BWAPI::Unit u) : NonArmyWrapper(u){};
    ~WorkerWrapper() = default;

    BWEM::Ressource* getCurrentResource(){return currRes;}
    void onFrame() override;
    void displayInfo() override;
protected:
    BWAPI::TilePosition getBlockOfSize(BWAPI::UnitType type, BWAPI::TilePosition pos);
    Jobs currJob = Jobs::None;
    BWEM::Ressource* currRes = nullptr;
};