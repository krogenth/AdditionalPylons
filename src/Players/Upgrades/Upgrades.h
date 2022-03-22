#pragma once
#include <map>

#include <BWAPI.h>

class Upgrades {
public:
	Upgrades(BWAPI::Player _player);

	void onFrame();

	/*
	Returns the damage of the specified BWAPI::WeaponType for the specified player
	@returns
		@retval int of the weapon's damage
	*/
	int getWeaponDamage(BWAPI::WeaponType weapon);

	/*
	Returns the range of the specified BWAPI::WeaponType for the specified player
	@returns
		@retval int of the weapon's range
	*/
	int getWeaponRange(BWAPI::WeaponType weapon);

	/*
	Returns the weapon cooldown of the specified BWAPI::UnitType for the specified player
	@returns
		@retval int of the unit's weapon cooldown
	*/
	int getUnitCooldown(BWAPI::UnitType type);

	/*
	Returns the armor of the specified BWAPI::UnitType for the specified player
	@returns
		@retval int of the unit's armor
	*/
	int getUnitArmor(BWAPI::UnitType type);

	/*
	Returns the sight range of the specified BWAPI::UnitType for the specified player
	@returns
		@retval int of the unit's sight range
	*/
	int getUnitSight(BWAPI::UnitType type);

	/*
	Returns the maximum movement speed of the specified BWAPI::UnitType for the specified player
	@returns
		@retval int of the unit's maximum movement speed
	*/
	double getUnitSpeed(BWAPI::UnitType type);

	/*
	Returns if the specified BWAPI::TechType has been researched by the specified player
	@returns
		@retval bool on if the specified player has researched the specified tech
	*/
	bool hasResearchedTech(BWAPI::TechType tech);

	/*
	Returns the upgrade level of the specified BWAPI::UpgradeType for the specified player
	@returns
		@retval int of the upgrade's level
	*/
	int getUpgradeLevel(BWAPI::UpgradeType upgrade);

protected:
	BWAPI::Player player = nullptr;
	std::map<BWAPI::WeaponType, int> weaponDamage;
	std::map<BWAPI::WeaponType, int> weaponRange;
	std::map<BWAPI::UnitType, int> unitCooldown;
	std::map<BWAPI::UnitType, double> unitSpeed;
	std::map<BWAPI::UnitType, int> unitArmor;
	std::map<BWAPI::UnitType, int> unitSight;
	std::map<BWAPI::TechType, bool> researchedTech;
	std::map<BWAPI::UpgradeType, int> upgradeLevel;
};