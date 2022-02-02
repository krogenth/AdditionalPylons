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

void Player::DisplayInfo() {
	int countUnit = 0;
	int countBuilding = 0;

	for (std::pair<int, BWAPI::Unit> unit : units) {
		countUnit++;
	}

	for (std::pair<int, BWAPI::Unit> building : buildings) {
		countBuilding++;
	}

	std::cout << "# of units that " + ourRace.toString() + "control: " + std::to_string(countUnit) << std::endl;
	std::cout << "# of buildings that " + ourRace.toString() + "control: " + std::to_string(countBuilding) << std::endl;
}

void Player::PrintRace() {
	std::cout << "This player's race is: " + ourRace.toString() << std::endl;
}
