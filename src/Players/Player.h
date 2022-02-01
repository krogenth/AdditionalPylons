#pragma once

#include <BWAPI.h>
#include <unordered_map>

class Player
{
private:
    std::unordered_map<int, BWAPI::Unit> units;
    std::unordered_map<int, BWAPI::Unit> buildings;
    BWAPI::Race ourRace;
    BWAPI::Race enemyRace;

public:
    void onStart();
    void onFrame();
    void onNukeDetect(BWAPI::Position target);
    void onUnitEvade(BWAPI::Unit unit);
    void onUnitHide(BWAPI::Unit unit);
    void onUnitCreate(BWAPI::Unit unit);
    void onUnitDestroy(BWAPI::Unit unit);
    void onUnitMorph(BWAPI::Unit unit);
    void onUnitRenegade(BWAPI::Unit unit);
    void onUnitComplete(BWAPI::Unit unit);
    void onUnitDiscover(BWAPI::Unit unit);
    BWAPI::Race returnRace();
    void DisplayInfo();
};

static Player player = Player();
static Player enemy = Player();