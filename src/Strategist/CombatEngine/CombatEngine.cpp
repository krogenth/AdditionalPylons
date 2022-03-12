#include "./CombatEngine.h"

void setCombatStats(const BWAPI::Position& center, const int radius) {
	if (center.isValid()) {
		int topx = center.x - radius;
		int topy = center.y + radius;
		int botx = center.x + radius;
		int boty = center.y - radius;
		BWAPI::Position topLeft = BWAPI::Position(topx, topy);
		BWAPI::Position botRight = BWAPI::Position(botx, boty);
		if (topLeft.isValid() && botRight.isValid()) {
			std::unordered_map<int, BWAPI::Unit> enemyTroops = enemy.getUnitsByArea(topLeft, botRight);
			std::unordered_map<int, BWAPI::Unit> myTroops = player.getUnitsByArea(topLeft, botRight);
			std::unordered_map<int, BWAPI::Unit>::iterator it = enemyTroops.begin();
			while (it != enemyTroops.end()) {
				if (it->second->isFlying()) {
					CombatEngine::enemyAir.numTroops++;
					BWAPI::UnitType troop = it->second->getType();
					BWAPI::WeaponType airWeapon = troop.airWeapon();
					BWAPI::WeaponType groundWeapon = troop.groundWeapon();
					int airDps = ((airWeapon.damageAmount() * airWeapon.damageFactor()) / airWeapon.damageCooldown());
					int groundDps = ((groundWeapon.damageAmount() * groundWeapon.damageFactor()) / groundWeapon.damageCooldown());
					CombatEngine::enemyAir.airDPS += airDps;
					CombatEngine::enemyAir.groundDPS += groundDps;
					CombatEngine::enemyAir.maxShield += troop.maxShields();
					CombatEngine::enemyAir.maxHealth += troop.maxHitPoints();
					CombatEngine::enemyAir.currHealth += it->second->getHitPoints();
					CombatEngine::enemyAir.currShield += it->second->getShields();
				}
				else {
					CombatEngine::enemyGround.numTroops++;
					BWAPI::UnitType troop = it->second->getType();
					BWAPI::WeaponType airWeapon = troop.airWeapon();
					BWAPI::WeaponType groundWeapon = troop.groundWeapon();
					int airDps = ((airWeapon.damageAmount() * airWeapon.damageFactor()) / airWeapon.damageCooldown());
					int groundDps = ((groundWeapon.damageAmount() * groundWeapon.damageFactor()) / groundWeapon.damageCooldown());
					CombatEngine::enemyGround.airDPS += airDps;
					CombatEngine::enemyGround.groundDPS += groundDps;
					CombatEngine::enemyGround.maxShield += troop.maxShields();
					CombatEngine::enemyGround.maxHealth += troop.maxHitPoints();
					CombatEngine::enemyGround.currHealth += it->second->getHitPoints();
					CombatEngine::enemyGround.currShield += it->second->getShields();
				}
			}
			it = myTroops.begin();
			while (it != myTroops.end()) {
				if (it->second->isFlying()) {
					CombatEngine::myAir.numTroops++;
					BWAPI::UnitType troop = it->second->getType();
					BWAPI::WeaponType airWeapon = troop.airWeapon();
					BWAPI::WeaponType groundWeapon = troop.groundWeapon();
					int airDps = ((airWeapon.damageAmount() * airWeapon.damageFactor()) / airWeapon.damageCooldown());
					int groundDps = ((groundWeapon.damageAmount() * groundWeapon.damageFactor()) / groundWeapon.damageCooldown());
					CombatEngine::myAir.airDPS += airDps;
					CombatEngine::myAir.groundDPS += groundDps;
					CombatEngine::myAir.maxShield += troop.maxShields();
					CombatEngine::myAir.maxHealth += troop.maxHitPoints();
					CombatEngine::myAir.currHealth += it->second->getHitPoints();
					CombatEngine::myAir.currShield += it->second->getShields();
				}
				else {
					CombatEngine::myGround.numTroops++;
					BWAPI::UnitType troop = it->second->getType();
					BWAPI::WeaponType airWeapon = troop.airWeapon();
					BWAPI::WeaponType groundWeapon = troop.groundWeapon();
					int airDps = ((airWeapon.damageAmount() * airWeapon.damageFactor()) / airWeapon.damageCooldown());
					int groundDps = ((groundWeapon.damageAmount() * groundWeapon.damageFactor()) / groundWeapon.damageCooldown());
					CombatEngine::myGround.airDPS += airDps;
					CombatEngine::myGround.groundDPS += groundDps;
					CombatEngine::myGround.maxShield += troop.maxShields();
					CombatEngine::myGround.maxHealth += troop.maxHitPoints();
					CombatEngine::myGround.currHealth += it->second->getHitPoints();
					CombatEngine::myGround.currShield += it->second->getShields();
				}
			}
		}
		else {
			throw std::invalid_argument("Area is not valid");
		}
	}
	else {
		throw std::invalid_argument("Center value is not valid");
	}
}

bool playerWins(const BWAPI::Position& center, const int radius) {
	setCombatStats(center, radius);
	int enemyTotalGroundDPS = CombatEngine::enemyGround.groundDPS + CombatEngine::enemyAir.groundDPS;
	int enemyTotalAirDPS = CombatEngine::enemyGround.airDPS + CombatEngine::enemyAir.airDPS;
	int myTotalGroundDPS = CombatEngine::myGround.groundDPS + CombatEngine::myAir.groundDPS;
	int myTotalAirDPS = CombatEngine::myGround.airDPS + CombatEngine::myAir.airDPS;
	//for now we'll treat the shields as part of the current health bar since we are handling damage for them the same way
	CombatEngine::myGround.currHealth += CombatEngine::myGround.currShield;
	CombatEngine::myAir.currHealth += CombatEngine::myAir.currShield;
	CombatEngine::enemyGround.currHealth += CombatEngine::enemyGround.currShield;
	CombatEngine::enemyAir.currHealth += CombatEngine::enemyAir.currShield;
	while (!(CombatEngine::myAir.currHealth <= 0) || !(CombatEngine::myGround.currHealth <= 0)) {
		if (CombatEngine::enemyAir.currHealth <= 0 && CombatEngine::enemyGround.currHealth <= 0) {
			return true;
		}
		CombatEngine::myGround.currHealth -= enemyTotalGroundDPS;
		CombatEngine::myAir.currHealth -= enemyTotalAirDPS;
		CombatEngine::enemyGround.currHealth -= myTotalGroundDPS;
		CombatEngine::enemyAir.currHealth -= myTotalAirDPS;
		
	}
	return false;
}