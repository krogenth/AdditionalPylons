#pragma once
#include "./NonArmyWrapper.h"

#include "../../Strategist/Strategist.h"

class OverlordWrapper : public NonArmyWrapper {
public:
    OverlordWrapper(BWAPI::Unit u) : NonArmyWrapper(u) {};
    ~OverlordWrapper() = default;

    void onFrame() override;
    void displayInfo() override;

private:
    PlayDecision play = PlayDecision::none;
};