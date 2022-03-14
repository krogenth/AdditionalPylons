#pragma once

#include <BWAPI.h>
#include "../../Players/Player.h"

struct combatStats {
	int numTroops=0;
	double maxHealth=0;
	double currHealth=0;
	double maxShield=0;
	double currShield=0;
	int avgArmor=0;
	int avgAlt=0;
	double airDPS=0;
	double groundDPS=0;
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

	std::tuple<double,double> calcDPS(BWAPI::UnitType troop);	
	playerStats playerSt;
	playerStats enemySt;
};