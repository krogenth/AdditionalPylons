#pragma once

#include <BWAPI.h>
#include "../../Players/Player.h"

struct combatStats {
	int numTroops;
	int maxHealth;
	int currHealth;
	int maxShield;
	int currShield;
	int avgArmor;
	int avgAlt;
	double airDPS;
	double groundDPS;
};

class CombatEngine {
private:
	combatStats enemyAir;
	combatStats enemyGround;
	combatStats myAir;
	combatStats myGround;
public:
	CombatEngine();
	void setCombatTroops(const BWAPI::Position & center, const int radius);

};