#include "./CombatEngine.h"

std::tuple<double,double> CombatEngine::calcDPS(BWAPI::UnitType troop) {
	BWAPI::WeaponType airWeapon = troop.airWeapon();
	BWAPI::WeaponType groundWeapon = troop.groundWeapon();
	double airDps = ((airWeapon.damageAmount() * airWeapon.damageFactor()) / airWeapon.damageCooldown());
	double groundDps = ((groundWeapon.damageAmount() * groundWeapon.damageFactor()) / groundWeapon.damageCooldown());
	auto retVal = std::make_tuple(airDps,groundDps);
	return retVal;
}

void CombatEngine::setCombatStats(const BWAPI::Position& center, const int radius) {
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
			for (const auto& [key, value] : enemyTroops){
				BWAPI::UnitType troop = value->getType();
				auto dpsVals = calcDPS(troop);
				if (value->isFlying()) {
					CombatEngine::enemySt.air.numTroops++;
					CombatEngine::enemySt.air.airDPS += std::get<0>(dpsVals);
					CombatEngine::enemySt.air.groundDPS += std::get<1>(dpsVals);
					CombatEngine::enemySt.air.maxShield += troop.maxShields();
					CombatEngine::enemySt.air.maxHealth += troop.maxHitPoints();
					CombatEngine::enemySt.air.currHealth += value->getHitPoints();
					CombatEngine::enemySt.air.currShield += value->getShields();
				}
				else {
					CombatEngine::enemySt.ground.numTroops++;
					CombatEngine::enemySt.ground.airDPS += std::get<0>(dpsVals);
					CombatEngine::enemySt.ground.groundDPS += std::get<1>(dpsVals);
					CombatEngine::enemySt.ground.maxShield += troop.maxShields();
					CombatEngine::enemySt.ground.maxHealth += troop.maxHitPoints();
					CombatEngine::enemySt.ground.currHealth += value->getHitPoints();
					CombatEngine::enemySt.ground.currShield += value->getShields();
				}
			}
			for (const auto& [key, value] : myTroops) {
				BWAPI::UnitType troop = value->getType();
				auto dpsVals = calcDPS(troop);
				if (value->isFlying()) {
					CombatEngine::playerSt.air.numTroops++;
					CombatEngine::playerSt.air.airDPS += std::get<0>(dpsVals);
					CombatEngine::playerSt.air.groundDPS += std::get<1>(dpsVals);
					CombatEngine::playerSt.air.maxShield += troop.maxShields();
					CombatEngine::playerSt.air.maxHealth += troop.maxHitPoints();
					CombatEngine::playerSt.air.currHealth += value->getHitPoints();
					CombatEngine::playerSt.air.currShield += value->getShields();
				}
				else {
					CombatEngine::playerSt.ground.numTroops++;
					CombatEngine::playerSt.ground.airDPS += std::get<0>(dpsVals);
					CombatEngine::playerSt.ground.groundDPS += std::get<1>(dpsVals);
					CombatEngine::playerSt.ground.maxShield += troop.maxShields();
					CombatEngine::playerSt.ground.maxHealth += troop.maxHitPoints();
					CombatEngine::playerSt.ground.currHealth += value->getHitPoints();
					CombatEngine::playerSt.ground.currShield += value->getShields();
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

bool CombatEngine::playerWins(const BWAPI::Position& center, const int radius) {
	setCombatStats(center, radius);
	double enemyTotalGroundDPS = CombatEngine::enemySt.ground.groundDPS + CombatEngine::enemySt.air.groundDPS;
	double enemyTotalAirDPS = CombatEngine::enemySt.ground.airDPS + CombatEngine::enemySt.air.airDPS;
	double myTotalGroundDPS = CombatEngine::playerSt.ground.groundDPS + CombatEngine::playerSt.air.groundDPS;
	double myTotalAirDPS = CombatEngine::playerSt.ground.airDPS + CombatEngine::playerSt.air.airDPS;
	//for now we'll treat the shields as part of the current health bar since we are handling damage for them the same way
	CombatEngine::playerSt.ground.currHealth += CombatEngine::playerSt.ground.currShield;
	CombatEngine::playerSt.air.currHealth += CombatEngine::playerSt.air.currShield;
	CombatEngine::enemySt.ground.currHealth += CombatEngine::enemySt.ground.currShield;
	CombatEngine::enemySt.air.currHealth += CombatEngine::enemySt.air.currShield;
	while (!(CombatEngine::playerSt.air.currHealth <= 0) || !(CombatEngine::playerSt.ground.currHealth <= 0)) {
		if (CombatEngine::enemySt.air.currHealth <= 0 && CombatEngine::enemySt.ground.currHealth <= 0) {
			return true;
		}
		CombatEngine::playerSt.ground.currHealth -= enemyTotalGroundDPS;
		CombatEngine::playerSt.air.currHealth -= enemyTotalAirDPS;
		CombatEngine::enemySt.ground.currHealth -= myTotalGroundDPS;
		CombatEngine::enemySt.air.currHealth -= myTotalAirDPS;
		
	}
	return false;
}

