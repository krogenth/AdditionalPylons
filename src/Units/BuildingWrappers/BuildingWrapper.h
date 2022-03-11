#pragma once
#include "../UnitWrapper.h"

class BuildingWrapper : public UnitWrapper {
public:
    BuildingWrapper(BWAPI::Unit u) : UnitWrapper(u) {};
    ~BuildingWrapper() = default;

    virtual void onFrame() override;
    virtual void displayInfo() override;
    
protected:
    bool hasOrder();

    BWAPI::UpgradeType upgradeOrder;
};