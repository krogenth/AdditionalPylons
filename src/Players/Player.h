#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include <BWAPI.h>

#include "../Units/Units.h"

class Player {
private:
    std::unordered_map<int, std::unique_ptr<ArmyWrapper>> armyUnits;
    std::unordered_map<int, std::unique_ptr<NonArmyWrapper>> nonArmyUnits;
    std::unordered_map<int, std::unique_ptr<BuildingWrapper>> buildingUnits;
    std::unordered_map<int, std::unique_ptr<UnitWrapper>> allUnits;
    BWAPI::Race playerRace;
    Player() = default;

public:
    Player(const Player&) = delete;
    Player(const Player&&) = delete;
    /*
    Returns the current instance of the player's instance
    @returns
        @retval Player& player instance reference
    */
    static Player& getPlayerInstance() {
        static Player instance;
        return instance;
    }
    /*
    Returns the current instance of the enemy's instance
    @returns
        @retval Player& enemy instance reference
    */
    static Player& getEnemyInstance() {
        static Player instance;
        return instance;
    }
    void onStart(BWAPI::Race race);
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
    BWAPI::Race getRace() { return this->playerRace; }
    void displayInfo(int x);
    /*
    Returns a map of all units in a specified area
    @returns
        @retval std::unordered_map<int, BWAPI::Unit> map of units in given area
    */
    std::unordered_map<int, BWAPI::Unit> getUnitsByArea(BWAPI::Position topLeft, BWAPI::Position botRight);
    /*
    Returns a map of all units of a specified type
    Returns all units if type is BWAPI::UnitTypes::Unknown
    @returns
        @retval std::unordered_map<int, BWAPI::Unit> map of units by unit type
    */
    std::unordered_map<int, BWAPI::Unit> getUnitsByType(BWAPI::UnitType type);
};