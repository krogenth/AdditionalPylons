#pragma once
#include "./ArmyWrapper.h"

class HydraliskWrapper : public ArmyWrapper {
public:
    HydraliskWrapper(BWAPI::Unit u) : ArmyWrapper(u) {};
    ~HydraliskWrapper() = default;

    void onFrame() override;
    void displayInfo() override;

protected:
    void handleScoutStrategy();
    void handleAttackStrategy();
    void handleDefenseStrategy();
};