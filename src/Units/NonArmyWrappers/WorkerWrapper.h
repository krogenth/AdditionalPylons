#pragma once
#include "./NonArmyWrapper.h"

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

    void onFrame() override;
    void displayInfo() override;
protected:
    BWAPI::TilePosition getBlockOfSize(BWAPI::UnitType type, BWAPI::TilePosition pos);
    Jobs currJob = Jobs::None;
};