#pragma once

#include <BWAPI.h>
#include "../../Players/Player.h"

struct combatStats {
	int numTroops=0;
	int maxHealth=0;
	int currHealth=0;
	int maxShield=0;
	int currShield=0;
	int avgArmor=0;
	int avgAlt=0;
	double airDPS=0;
	double groundDPS=0;
};

class CombatEngine {	
	
public:
	CombatEngine();
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
	static combatStats enemyAir;
	static combatStats enemyGround;
	static combatStats myAir;
	static combatStats myGround;
};