#pragma once
#include "./ArmyWrapper.h"

class ZerglingWrapper : public ArmyWrapper {
public:
    ZerglingWrapper(BWAPI::Unit u) : ArmyWrapper(u) {};
    ~ZerglingWrapper() = default;

    void onFrame() override;
    void displayInfo() override;
};