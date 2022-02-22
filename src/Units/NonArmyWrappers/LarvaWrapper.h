#pragma once
#include "./NonArmyWrapper.h"

class LarvaWrapper : public NonArmyWrapper {
public:
    LarvaWrapper(BWAPI::Unit u) : NonArmyWrapper(u) {};
    ~LarvaWrapper() = default;

    virtual void onFrame() override;
    virtual void displayInfo() override;
};