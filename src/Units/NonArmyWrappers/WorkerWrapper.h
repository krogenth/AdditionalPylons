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
    Jobs currJob = Jobs::None;
    BWAPI::UnitType buildOrder = BWAPI::UnitTypes::Unknown;
};