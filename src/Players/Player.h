#pragma once
#include <BWAPI.h>
#include <BWEB.h>
#include <bwem.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "../Units/Units.h"

class Player {
   private:
    std::unordered_map<int, std::unique_ptr<ArmyWrapper>> armyUnits;
    std::unordered_map<int, std::unique_ptr<NonArmyWrapper>> nonArmyUnits;
    std::unordered_map<int, std::unique_ptr<BuildingWrapper>> buildingUnits;
    std::unordered_map<int, std::unique_ptr<UnitWrapper>> allUnits;
    std::map<BWEM::Ressource*, int> allGeysers;
    std::map<BWEM::Ressource*, int> allMinerals;
    std::set<const BWEM::Area*> buildingAreas;
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

    const std::set<const BWEM::Area*>& getBuildingAreas() { return buildingAreas; };

    /*
    Returns the closest geyser or nullptr if no geyser found
    @returns
        @retval BWEM::Ressource* of the closest geyser or nullptr if no geyser found
    */
    BWEM::Ressource* getClosestGeyser(BWAPI::Position pos);

     /*
    Returns the closest mineral or nullptr if no mineral found
    @returns
        @retval BWEM::Ressource* of the closest mineral or nullptr if no mineral found
    */
    BWEM::Ressource* getClosestMineral(BWAPI::Position pos);

     /*
    Returns the ratio of allMinerals/allGeysers or float_max if allGeysers is 0
    @returns
        @retval float of the ratio of allMinerals/allGeysers or float_max if allGeysers is 0
    */
    float MineralGasRatio();
    /*
    
    */
    void AdjResWorkCount(BWEM::Ressource* res, int val);
};
