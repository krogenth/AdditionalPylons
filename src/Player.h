#ifndef PLAYER.H
#define PLAYER.H

#include <BWAPI.h>
#include <unordered_map>

class Player : public BWAPI::AIModule
{
public:
    void onStart() override;
    void onFrame() override;
    void onNukeDetect(BWAPI::Position target) override;
    void onUnitEvade(BWAPI::Unit unit) override;
    void onUnitHide(BWAPI::Unit unit) override;
    void onUnitCreate(BWAPI::Unit unit) override;
    void onUnitDestroy(BWAPI::Unit unit) override;
    void onUnitMorph(BWAPI::Unit unit) override;
    void onUnitRenegade(BWAPI::Unit unit) override;
    void onUnitComplete(BWAPI::Unit unit) override;
    void onUnitDiscover(BWAPI::Unit unit) override;
    BWAPI::Race returnRace();
    void DisplayInfo();
};

#endif