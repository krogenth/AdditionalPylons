#include "./Upgrades.h"

Upgrades::Upgrades(BWAPI::Player _player) {
	this->player = _player;
	this->weaponDamage.clear();
	this->weaponRange.clear();
	this->unitCooldown.clear();
	this->unitSpeed.clear();
	this->unitArmor.clear();
	this->unitSight.clear();
	this->researchedTech.clear();
	this->upgradeLevel.clear();
};

void Upgrades::onFrame() {
	// update all previously checked weapon damages
	for (auto& [key, value] : this->weaponDamage) {
		auto damage = this->player->damage(key);
		if (damage != value)
			value = damage;
	}

	// update all previously checked weapon ranges
	for (auto& [key, value] : this->weaponRange) {
		auto range = this->player->weaponMaxRange(key);
		if (range != value)
			value = range;
	}

	// update all previously checked unit weapon cooldowns
	for (auto& [key, value] : this->unitCooldown) {
		auto cooldown = this->player->weaponDamageCooldown(key);
		if (cooldown != value)
			value = cooldown;
	}

	// update all previously checked unit weapon cooldowns
	for (auto& [key, value] : this->unitArmor) {
		auto armor = this->player->armor(key);
		if (armor != value)
			value = armor;
	}

	// update all previously checked unit sight ranges
	for (auto& [key, value] : this->unitSight) {
		auto sight = this->player->sightRange(key);
		if (sight != value)
			value = sight;
	}

	// update all previously checked unit max speeds
	for (auto& [key, value] : this->unitSpeed) {
		auto speed = this->player->topSpeed(key);
		if (speed != value)
			value = speed;
	}

	// update all previously checked tech researched statuses
	for (auto& [key, value] : this->researchedTech) {
		auto researched = this->player->hasResearched(key);
		if (researched != value)
			value = researched;
	}

	// update all previously checked upgrade levels
	for (auto& [key, value] : this->upgradeLevel) {
		auto level = this->player->getUpgradeLevel(key);
		if (level != value)
			value = level;
	}
}

int Upgrades::getWeaponDamage(BWAPI::WeaponType weapon) {
	auto damageIter = this->weaponDamage.find(weapon);
	if (damageIter != this->weaponDamage.end())
		return damageIter->second;
	int damage = this->player->damage(weapon);
	this->weaponDamage[weapon] = damage;
	return damage;
}

int Upgrades::getWeaponRange(BWAPI::WeaponType weapon) {
	auto rangeIter = this->weaponRange.find(weapon);
	if (rangeIter != this->weaponRange.end())
		return rangeIter->second;
	int range = this->player->weaponMaxRange(weapon);
	this->weaponRange[weapon] = range;
	return range;
}

int Upgrades::getUnitCooldown(BWAPI::UnitType type) {
	auto cooldownIter = this->unitCooldown.find(type);
	if (cooldownIter != this->unitCooldown.end())
		return cooldownIter->second;
	int cooldown = this->player->weaponDamageCooldown(type);
	this->unitCooldown[type] = cooldown;
	return cooldown;
}

int Upgrades::getUnitArmor(BWAPI::UnitType type) {
	auto armorIter = this->unitArmor.find(type);
	if (armorIter != this->unitArmor.end())
		return armorIter->second;
	int armor = this->player->armor(type);
	this->unitArmor[type] = armor;
	return armor;
}

int Upgrades::getUnitSight(BWAPI::UnitType type) {
	auto sightIter = this->unitSight.find(type);
	if (sightIter != this->unitSight.end())
		return sightIter->second;
	int sight = this->player->sightRange(type);
	this->unitSight[type] = sight;
	return sight;
}

double Upgrades::getUnitSpeed(BWAPI::UnitType type) {
	auto speedIter = this->unitSpeed.find(type);
	if (speedIter != this->unitSpeed.end())
		return speedIter->second;
	double speed = this->player->topSpeed(type);
	this->unitSpeed[type] = speed;
	return speed;
}

bool Upgrades::hasResearchedTech(BWAPI::TechType tech) {
	auto techIter = this->researchedTech.find(tech);
	if (techIter != this->researchedTech.end())
		return techIter->second;
	bool researched = this->player->hasResearched(tech);
	this->researchedTech[tech] = researched;
	return researched;
}

int Upgrades::getUpgradeLevel(BWAPI::UpgradeType upgrade) {
	auto upgradeIter = this->upgradeLevel.find(upgrade);
	if (upgradeIter != this->upgradeLevel.end())
		return upgradeIter->second;
	int level = this->player->getUpgradeLevel(upgrade);
	this->upgradeLevel[upgrade] = level;
	return level;
}
