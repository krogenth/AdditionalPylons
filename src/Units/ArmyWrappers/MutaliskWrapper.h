#pragma once
#include "./ArmyWrapper.h"

class MutaliskWrapper : public ArmyWrapper {
public:
    MutaliskWrapper(BWAPI::Unit u) : ArmyWrapper(u) {};
    ~MutaliskWrapper() = default;

    void onFrame() override;
    void displayInfo() override;

protected:
    void handleScoutStrategy();
    void handleAttackStrategy();
    void handleDefenseStrategy();
};