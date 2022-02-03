#include "./Player.h"

//Auto assigns this player's race and sets enemies race to unknown
void Player::onStart(BWAPI::Race race) {
	units.clear();
	buildings.clear();
	ourRace = race;
	//enemyRace = BWAPI::Races::Unknown;
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
	if (ourRace == BWAPI::Races::Unknown)
		ourRace = unit->getType().getRace();

	if (unit->getType().isBuilding()) {
		buildings[unit->getID()] = unit;
	}
	else if (unit->getType().isWorker()) {
		units[unit->getID()] = unit;
	}
}

//When a unit is destroyed, removes unit from maps corresponding type
void Player::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getType().isBuilding()) {
		buildings.erase(unit->getID());
	}

	else if (unit->getType().isWorker()) {
		units.erase(unit->getID());
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

//Returns this player's race
BWAPI::Race Player::returnRace() {
	return ourRace;
}

void Player::DisplayInfo(int x, int y) {

	BWAPI::Broodwar->drawTextScreen(x, y, "# of units that %s control: %s", ourRace.toString(), std::to_string(units.size()));
	BWAPI::Broodwar->drawTextScreen(x, y, "# of units that %s control: %s", ourRace.toString(), std::to_string(buildings.size()));
}

void Player::PrintRace() {
	BWAPI::Broodwar->drawTextScreen(x, y, "This player's race is: %s", ourRace.toString());
}
