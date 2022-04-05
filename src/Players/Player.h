#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include <BWAPI.h>
#include <BWEB.h>
#include <bwem.h>

#include "./Upgrades/Upgrades.h"
#include "../Units/Units.h"

class Player {
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
    Returns a map of all units matching some predicate function
    The Predicate function should return true for units it return, and false for those to be excluded
    @returns
        @retval std::unordered_map<int, BWAPI::Unit> map of units
    */
    std::unordered_map<int, BWAPI::Unit> getUnitsByPredicate(std::function <bool(const std::shared_ptr<UnitWrapper>)> predicate);

    /*
    Returns a map of count of each BWAPI::UnitType owned by the player
    @returns
        @retval std::map<BWAPI::UnitType, int> map of count of each BWAPI::UnitType
    */
    std::map<BWAPI::UnitType, int> getUnitCount();

    /*
    Returns a set of all BWEM::Area*'s associated with the player
    @returns
        @retval std::set<const BWEM::Area*> set of all player owned BWEM::Area*
    */
    const std::set<const BWEM::Area*>& getBuildingAreas() { return buildingAreas; };

    /*
    Returns the closest geyser or nullptr if no geyser found
    @returns
        @retval BWEM::Ressource* to the closest resource
        @retval nullptr if no geyser was found
    */
    BWEM::Ressource* getClosestGeyser(BWAPI::Position pos);

    /*
    Returns the closest mineral or nullptr if no mineral found
    @returns
        @retval BWEM::Ressource* to the closest resource
        @retval nullptr if no mineral was found
    */
    BWEM::Ressource* getClosestMineral(BWAPI::Position pos);

    /*
    Returns the ratio of gas:mineral workers or FLOAT_MAX if the number of mineral workers is 0
    @returns
        @retval float of the ratio of gas:mineral workers
    */
    float getGasToMineralWorkerRatio();

    /*
    Adjusts the amount of workers for a given resource.
    res as the resource, val as the amount to adjust it by.
    */
    void adjustResourceWorkerCount(BWEM::Ressource* res, int val);

    /*
    Returns the number of army units the player instance has
    @returns
        @retval int of the number of army units associated with the player
    */
    int getArmyUnitCount() { return this->armyUnits.size(); }

private:
    Player() = default;

    /*
    Returns the closest ground distance based player associated resource to a position
    @returns
        @retval BWEM::Ressource* to the closest resource
        @retval nullptr if no resource was found
    */
    BWEM::Ressource* getClosestResource(BWAPI::Position pos, const std::map<BWEM::Ressource*, int>& resources);

    std::unordered_map<int, std::shared_ptr<ArmyWrapper>> armyUnits;
    std::unordered_map<int, std::shared_ptr<NonArmyWrapper>> nonArmyUnits;
    std::unordered_map<int, std::shared_ptr<BuildingWrapper>> buildingUnits;
    std::unordered_map<int, std::shared_ptr<UnitWrapper>> allUnits;
    std::map<BWEM::Ressource*, int> allGeysers;
    std::map<BWEM::Ressource*, int> allMinerals;
    std::set<const BWEM::Area*> buildingAreas;
    BWAPI::Race playerRace;
};

namespace PlayerUpgrades {
    void onStart();
    void onFrame();
    /*
    Returns an Upgrades tracking class pointer based on the BWAPI::Player given
    @returns
        @retval std::shared_ptr<Upgrades> for the BWAPI::Player given
    */
    std::shared_ptr<Upgrades> getPlayerUpgrades(BWAPI::Player player);
};