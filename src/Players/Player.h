#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include <BWAPI.h>

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
    void onStart(BWAPI::Player player);
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

	/*
	Returns the damage of the specified BWAPI::WeaponType
	@returns
		@retval int of the weapon's damage
	*/
	int getWeaponDamage(BWAPI::WeaponType weapon) { this->upgrades.getWeaponDamage(weapon); };

	/*
	Returns the range of the specified BWAPI::WeaponType
	@returns
		@retval int of the weapon's range
	*/
	int getWeaponRange(BWAPI::WeaponType weapon) { this->upgrades.getWeaponRange(weapon); };

	/*
	Returns the weapon cooldown of the specified BWAPI::UnitType
	@returns
		@retval int of the unit's weapon cooldown
	*/
	int getUnitCooldown(BWAPI::UnitType type) { this->upgrades.getUnitCooldown(type); };

	/*
	Returns the armor of the specified BWAPI::UnitType
	@returns
		@retval int of the unit's armor
	*/
	int getUnitArmor(BWAPI::UnitType type) { this->upgrades.getUnitArmor(type); };

	/*
	Returns the sight range of the specified BWAPI::UnitType
	@returns
		@retval int of the unit's sight range
	*/
	int getUnitSight(BWAPI::UnitType type) { this->upgrades.getUnitSight(type); };

	/*
	Returns the maximum movement speed of the specified BWAPI::UnitType
	@returns
		@retval int of the unit's maximum movement speed
	*/
	double getUnitSpeed(BWAPI::UnitType type) { this->upgrades.getUnitSpeed(type); };

	/*
	Returns if the specified BWAPI::TechType has been researched
	@returns
		@retval bool on if the specified player has researched the specified tech
	*/
	bool hasResearchedTech(BWAPI::TechType tech) { this->upgrades.hasResearchedTech(tech); };

	/*
	Returns the upgrade level of the specified BWAPI::UpgradeType
	@returns
		@retval int of the upgrade's level
	*/
	int getUpgradeLevel(BWAPI::UpgradeType upgrade) { this->upgrades.getUpgradeLevel(upgrade); };

protected:
    Player() = default;

    Upgrades upgrades = Upgrades();
    std::unordered_map<int, std::unique_ptr<ArmyWrapper>> armyUnits;
    std::unordered_map<int, std::unique_ptr<NonArmyWrapper>> nonArmyUnits;
    std::unordered_map<int, std::unique_ptr<BuildingWrapper>> buildingUnits;
    std::unordered_map<int, std::unique_ptr<UnitWrapper>> allUnits;
    BWAPI::Player player = nullptr;
    BWAPI::Race playerRace;
};