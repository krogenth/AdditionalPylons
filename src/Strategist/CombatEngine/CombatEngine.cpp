#include "./CombatEngine.h"

std::tuple<double,double> CombatEngine::calcDPS(BWAPI::UnitType troop) {
	BWAPI::WeaponType airWeapon = troop.airWeapon();
	BWAPI::WeaponType groundWeapon = troop.groundWeapon();
	double airDps = ((airWeapon.damageAmount() * airWeapon.damageFactor()) / airWeapon.damageCooldown());
	double groundDps = ((groundWeapon.damageAmount() * groundWeapon.damageFactor()) / groundWeapon.damageCooldown());
	auto retVal = std::make_tuple(airDps,groundDps);
	return retVal;
}

void CombatEngine::resetCombatStats(combatStats stats)
{
	stats.numTroops = 0;
	stats.currHealth = 0;
	stats.currShield = 0;
	stats.avgHealth = 0;
	stats.avgShield = 0;
	stats.damageTaken = 0;
	stats.totalArmor = 0;
	stats.avgAlt = 0;
	stats.airDPS = 0;
	stats.groundDPS = 0;
	stats.groundNumHits = 0;
	stats.airNumHits = 0;
	stats.avgRange = 0;
}

void CombatEngine::applyHitChance()
{
	//account for altitude
	if ((playerSt.air.avgAlt - enemySt.air.avgAlt) >= 1) {
		enemySt.air.airDPS *= 0.53125;
	}
	else if ((playerSt.air.avgAlt - enemySt.air.avgAlt) <= -1) {
		playerSt.air.airDPS *= 0.53125;
	}
	if ((playerSt.air.avgAlt - enemySt.ground.avgAlt) >= 1) {
		enemySt.ground.airDPS *= 0.53125;
	}
	else if ((playerSt.air.avgAlt - enemySt.ground.avgAlt) <= -1) {
		playerSt.air.groundDPS *= 0.53125;
	}
	if ((playerSt.ground.avgAlt - enemySt.air.avgAlt) >= 1) {
		enemySt.air.groundDPS *= 0.53125;
	}
	else if ((playerSt.ground.avgAlt - enemySt.air.avgAlt) <= -1) {
		playerSt.ground.airDPS *= 0.53125;
	}
	if ((playerSt.ground.avgAlt - enemySt.ground.avgAlt) >= 1) {
		enemySt.ground.groundDPS *= 0.53125;
	}
	else if ((playerSt.ground.avgAlt - enemySt.ground.avgAlt) <= -1) {
		playerSt.ground.groundDPS *= 0.53125;
	}
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
			resetCombatStats(playerSt.air);
			resetCombatStats(playerSt.ground);
			resetCombatStats(enemySt.air);
			resetCombatStats(enemySt.ground);
			for (const auto& [key, value] : enemyTroops) {
				BWAPI::UnitType troop = value->getType();
				auto dpsVals = calcDPS(troop);
				if (value->isFlying()) {
					enemySt.air.numTroops++;
					enemySt.air.airDPS += std::get<0>(dpsVals);
					enemySt.air.groundDPS += std::get<1>(dpsVals);
					enemySt.air.currHealth += value->getHitPoints();
					enemySt.air.currShield += value->getShields();
					enemySt.air.airNumHits += troop.airWeapon().damageFactor();
					enemySt.air.groundNumHits += troop.groundWeapon().damageFactor();
					enemySt.air.totalArmor += troop.armor();
					enemySt.air.avgAlt += BWAPI::Broodwar->getGroundHeight(value->getTilePosition());
				}
				else {
					enemySt.ground.numTroops++;
					enemySt.ground.airDPS += std::get<0>(dpsVals);
					enemySt.ground.groundDPS += std::get<1>(dpsVals);
					enemySt.ground.currHealth += value->getHitPoints();
					enemySt.ground.currShield += value->getShields();
					enemySt.ground.airNumHits += troop.airWeapon().damageFactor();
					enemySt.ground.groundNumHits += troop.groundWeapon().damageFactor();
					enemySt.ground.totalArmor += troop.armor();
					enemySt.ground.avgAlt += BWAPI::Broodwar->getGroundHeight(value->getTilePosition());
				}
			}
			enemySt.air.avgAlt /= enemySt.air.numTroops;
			enemySt.ground.avgAlt /= enemySt.ground.numTroops;
			enemySt.air.avgHealth = enemySt.air.currHealth / enemySt.air.numTroops;
			enemySt.ground.avgHealth = enemySt.ground.currHealth / enemySt.ground.numTroops;
			enemySt.air.avgShield = enemySt.air.currShield / enemySt.air.numTroops;
			enemySt.ground.avgShield = enemySt.ground.currShield / enemySt.ground.numTroops;
			for (const auto& [key, value] : myTroops) {
				BWAPI::UnitType troop = value->getType();
				auto dpsVals = calcDPS(troop);
				if (value->isFlying()) {
					playerSt.air.numTroops++;
					playerSt.air.airDPS += std::get<0>(dpsVals);
					playerSt.air.groundDPS += std::get<1>(dpsVals);
					playerSt.air.currHealth += value->getHitPoints();
					playerSt.air.currShield += value->getShields();
					playerSt.air.airNumHits += troop.airWeapon().damageFactor();
					playerSt.air.groundNumHits += troop.groundWeapon().damageFactor();
					playerSt.air.totalArmor += troop.armor();
					playerSt.air.avgAlt += BWAPI::Broodwar->getGroundHeight(value->getTilePosition());
				}
				else {
					playerSt.ground.numTroops++;
					playerSt.ground.airDPS += std::get<0>(dpsVals);
					playerSt.ground.groundDPS += std::get<1>(dpsVals);
					playerSt.ground.currHealth += value->getHitPoints();
					playerSt.ground.currShield += value->getShields();
					playerSt.ground.airNumHits += troop.airWeapon().damageFactor();
					playerSt.ground.groundNumHits += troop.groundWeapon().damageFactor();
					playerSt.ground.totalArmor += troop.armor();
					playerSt.ground.avgAlt += BWAPI::Broodwar->getGroundHeight(value->getTilePosition());
				}
			}
			playerSt.air.avgAlt /= playerSt.air.numTroops;
			playerSt.ground.avgAlt /= playerSt.ground.numTroops;
			playerSt.air.avgHealth = playerSt.air.currHealth / playerSt.air.numTroops;
			playerSt.ground.avgHealth = playerSt.ground.currHealth / playerSt.ground.numTroops;
			playerSt.air.avgShield = playerSt.air.currShield / playerSt.air.numTroops;
			playerSt.ground.avgShield = playerSt.ground.currShield / playerSt.ground.numTroops;
			//reduce damage from armor
			playerSt.air.airDPS -= (playerSt.air.airNumHits * enemySt.air.totalArmor);
			playerSt.ground.airDPS -= (playerSt.ground.airNumHits * enemySt.air.totalArmor);
			playerSt.air.groundDPS -= (playerSt.air.groundNumHits * enemySt.ground.totalArmor);
			playerSt.ground.groundDPS -= (playerSt.ground.groundNumHits * enemySt.ground.totalArmor);
			enemySt.air.airDPS -= (enemySt.air.airNumHits * playerSt.air.totalArmor);
			enemySt.ground.airDPS -= (enemySt.ground.airNumHits * playerSt.air.totalArmor);
			enemySt.air.groundDPS -= (enemySt.air.groundNumHits * playerSt.ground.totalArmor);
			enemySt.ground.groundDPS -= (enemySt.ground.groundNumHits * playerSt.ground.totalArmor);
			applyHitChance();
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
	
	while (!(CombatEngine::playerSt.air.currHealth <= 0) || !(CombatEngine::playerSt.ground.currHealth <= 0)) {
		if (CombatEngine::enemySt.air.currHealth <= 0 && CombatEngine::enemySt.ground.currHealth <= 0) {
			return true;
		}
		//player air troops damage
		if (playerSt.air.airDPS>playerSt.air.groundDPS && enemySt.air.currHealth>0) {
			if (enemySt.air.currShield>0) {
				enemySt.air.currShield -= playerSt.air.airDPS;
				if (enemySt.air.currShield < 0) { enemySt.air.currHealth += enemySt.air.currShield; }
			}
			else {
				enemySt.air.currHealth -= playerSt.air.airDPS;
			}
			enemySt.air.damageTaken += playerSt.air.airDPS;
		}
		else {
			if (enemySt.ground.currShield > 0) {
				enemySt.ground.currShield -= playerSt.air.groundDPS;
				if (enemySt.ground.currShield < 0) { enemySt.ground.currHealth += enemySt.ground.currShield; }
			}
			else {
				enemySt.ground.currHealth -= playerSt.air.groundDPS;
			}
			enemySt.ground.damageTaken += playerSt.air.groundDPS;
		}
		//player ground troops damage
		if (playerSt.ground.airDPS > playerSt.ground.groundDPS && enemySt.air.currHealth > 0) {
			if (enemySt.air.currShield > 0) {
				enemySt.air.currShield -= playerSt.ground.airDPS;
				if (enemySt.air.currShield < 0) { enemySt.air.currHealth += enemySt.air.currShield; }
			}
			else {
				enemySt.air.currHealth -= playerSt.ground.airDPS;
			}
			enemySt.air.damageTaken += playerSt.ground.airDPS;
		}
		else {
			if (enemySt.ground.currShield > 0) {
				enemySt.ground.currShield -= playerSt.ground.groundDPS;
				if (enemySt.ground.currShield < 0) { enemySt.ground.currHealth += enemySt.ground.currShield; }
			}
			else {
				enemySt.ground.currHealth -= playerSt.ground.groundDPS;
			}
			enemySt.ground.damageTaken += playerSt.ground.groundDPS;
		}
		//enemy air troops damage
		if (enemySt.air.airDPS > enemySt.air.groundDPS && playerSt.air.currHealth > 0) {
			if (playerSt.air.currShield > 0) {
				playerSt.air.currShield -= enemySt.air.airDPS;
				if (playerSt.air.currShield < 0) { playerSt.air.currHealth += playerSt.air.currShield; }
			}
			else {
				playerSt.air.currHealth -= enemySt.air.airDPS;
			}
			playerSt.air.damageTaken += enemySt.air.airDPS;
		}
		else {
			if (playerSt.ground.currShield > 0) {
				playerSt.ground.currShield -= enemySt.air.groundDPS;
				if (playerSt.ground.currShield < 0) { playerSt.ground.currHealth += playerSt.ground.currShield; }
			}
			else {
				playerSt.ground.currHealth -= enemySt.air.groundDPS;
			}
			playerSt.ground.damageTaken += enemySt.air.groundDPS;
		}
		//enemy ground troops damage
		if (enemySt.ground.airDPS > enemySt.ground.groundDPS && playerSt.air.currHealth > 0) {
			if (playerSt.air.currShield > 0) {
				playerSt.air.currShield -= enemySt.ground.airDPS;
				if (playerSt.air.currShield < 0) { playerSt.air.currHealth += playerSt.air.currShield; }
			}
			else {
				playerSt.air.currHealth -= enemySt.ground.airDPS;
			}
			playerSt.air.damageTaken += enemySt.ground.airDPS;
		}
		else {
			if (playerSt.ground.currShield > 0) {
				playerSt.ground.currShield -= enemySt.ground.groundDPS;
				if (playerSt.ground.currShield < 0) { playerSt.ground.currHealth += playerSt.ground.currShield; }
			}
			else {
				playerSt.ground.currHealth -= enemySt.ground.groundDPS;
			}
			playerSt.ground.damageTaken += enemySt.ground.groundDPS;
		}
		// Update dps values to account for units dying
		if ((playerSt.air.damageTaken >= (playerSt.air.avgHealth + playerSt.air.avgShield)) && (playerSt.air.numTroops > 0)) {
			playerSt.air.airDPS *= ((playerSt.air.numTroops - 1) / (playerSt.air.numTroops));
			playerSt.air.groundDPS *= ((playerSt.air.numTroops - 1) / (playerSt.air.numTroops));
			playerSt.air.numTroops--;
			playerSt.air.damageTaken = 0;
		}
		if ((playerSt.ground.damageTaken >= (playerSt.ground.avgHealth + playerSt.ground.avgShield)) && (playerSt.ground.numTroops > 0)) {
			playerSt.ground.airDPS *= ((playerSt.ground.numTroops - 1) / (playerSt.ground.numTroops));
			playerSt.ground.groundDPS *= ((playerSt.ground.numTroops - 1) / (playerSt.ground.numTroops));
			playerSt.ground.numTroops--;
			playerSt.ground.damageTaken = 0;
		}
		if ((enemySt.air.damageTaken >= (enemySt.air.avgHealth + enemySt.air.avgShield)) && (enemySt.air.numTroops > 0)) {
			enemySt.air.airDPS *= ((enemySt.air.numTroops - 1) / (enemySt.air.numTroops));
			enemySt.air.groundDPS *= ((enemySt.air.numTroops - 1) / (enemySt.air.numTroops));
			enemySt.air.numTroops--;
			enemySt.air.damageTaken = 0;
		}
		if ((enemySt.ground.damageTaken >= (enemySt.ground.avgHealth + enemySt.ground.avgShield)) && (enemySt.ground.numTroops > 0)) {
			enemySt.ground.airDPS *= ((enemySt.ground.numTroops - 1) / (enemySt.ground.numTroops));
			enemySt.ground.groundDPS *= ((enemySt.ground.numTroops - 1) / (enemySt.ground.numTroops));
			enemySt.ground.numTroops--;
			enemySt.ground.damageTaken = 0;
		}
	}
	return false;
}

