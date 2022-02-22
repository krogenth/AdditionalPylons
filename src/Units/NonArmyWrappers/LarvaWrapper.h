#pragma once
#include "./NonArmyWrapper.h"

class LarvaWrapper : public NonArmyWrapper {
public:
    LarvaWrapper(BWAPI::Unit u) : NonArmyWrapper(u) {};

    virtual void onFrame();
    virtual void displayInfo() {};
};