#pragma once
#include "./ArmyWrapper.h"

class LurkerWrapper : public ArmyWrapper {
public:
    LurkerWrapper(BWAPI::Unit u) : ArmyWrapper(u) {};
    ~LurkerWrapper() = default;

    void onFrame() override;
    void displayInfo() override;
};