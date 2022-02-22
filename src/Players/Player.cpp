#include "./Player.h"

//Auto assigns this player's race and sets enemies race to unknown
void Player::onStart(BWAPI::Race race) {
	this->units.clear();
	this->buildings.clear();
	this->playerRace = race;
}

void Player::onFrame() {

}

void Player::onNukeDetect(BWAPI::Position target) {

}

void Player::onUnitEvade(BWAPI::Unit unit) {

}

void Player::onUnitHide(BWAPI::Unit unit) {

}

//When a unit is created, adds unit to maps corresponding to type
void Player::onUnitCreate(BWAPI::Unit unit) {

	//If this is the first time seeing an enemy unit, we now know what race the enemy is
	if (this->playerRace == BWAPI::Races::Unknown)
		this->playerRace = unit->getType().getRace();

	if (unit->getType().isBuilding()) {
		this->buildings[unit->getID()] = unit;
	}
	else if (unit->getType().isWorker()) {
		this->units[unit->getID()] = unit;
	}
}

//When a unit is destroyed, removes unit from maps corresponding type
void Player::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getType().isBuilding()) {
		this->buildings.erase(unit->getID());
	}

	else if (unit->getType().isWorker()) {
		this->units.erase(unit->getID());
	}
}

void Player::onUnitMorph(BWAPI::Unit unit) {

}

void Player::onUnitRenegade(BWAPI::Unit unit) {

}

void Player::onUnitComplete(BWAPI::Unit unit) {

}

void Player::onUnitDiscover(BWAPI::Unit unit) {

}

//Displays player info (race, # of units, # of buildings)
void Player::displayInfo(int x) {
	std::string race;

	switch (this->playerRace) {
	case BWAPI::Races::Enum::Protoss: race = "Protoss"; break;
	case BWAPI::Races::Enum::Terran: race = "Terran"; break;
	case BWAPI::Races::Enum::Zerg: race = "Zerg"; break;
	default: race = "Unknown"; break;
	}

	BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Large);
	BWAPI::Broodwar->drawTextScreen(x, 50, "Race: %s", race.c_str());
	BWAPI::Broodwar->drawTextScreen(x, 65, "Units: %d", this->units.size());
	BWAPI::Broodwar->drawTextScreen(x, 80, "Buildings: %d", this->buildings.size());
	BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Default);
}
