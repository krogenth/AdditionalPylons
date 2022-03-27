#pragma once

#include <BWAPI.h>
#include "../../Players/Player.h"

struct combatStats {
	int numTroops = 0;
	double currHealth = 0;
	double currShield = 0;
	double avgHealth = 0;
	double avgShield = 0;
	double damageTaken = 0;
	int totalArmor = 0;
	double avgAlt = 0;
	double airDPS = 0;
	double groundDPS = 0;
	int groundNumHits = 0;
	int airNumHits = 0;
	double avgRange = 0;
};

struct playerStats {
	combatStats air;
	combatStats ground;
};

class CombatEngine {	
	
	
public:
	/*
		Sets the combatStats for enemy units and player units
	 */
	void setCombatStats(const BWAPI::Position& center, const int radius);
	/*
		Predicts who will win the battle and returns a bool
		@returns
			@retval bool true if player should win
			@retval bool false if enemy should win
	*/
	bool playerWins(const BWAPI::Position& center, const int radius);

	/*
		Calculates the DPS of a troop
		@returns
			@retval std::tuple<double airDPS,double groundDPS>
	*/
	std::tuple<double,double> calcDPS(BWAPI::UnitType troop);	

	/*
		Resets an instance of the combatStats structure to all 0's
	*/
	void resetCombatStats(combatStats stats);

	/*
		Applies the hit chance modifiers to both player and enemy DPS numbers
	*/
	void applyHitChance();
	playerStats playerSt;
	playerStats enemySt;
};