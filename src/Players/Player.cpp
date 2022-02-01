#include "./Player.h"

std::unordered_map<int, BWAPI::Unit> units;
std::unordered_map<int, BWAPI::Unit> buildings;
BWAPI::Race ourRace;
BWAPI::Race enemyRace;

//Auto assigns this player's race and sets enemies race to unknown
void Player::onStart() {
	ourRace = BWAPI::Broodwar->self()->getRace();
	enemyRace = BWAPI::Races::Unknown;
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
	//If this is the first time seeing an enemy unit, we now know what race the enemy is
	if (enemyRace == BWAPI::Races::Unknown)
		enemyRace = unit->getPlayer()->getRace();
}

//Returns this player's race
BWAPI::Race Player::returnRace() {
	return ourRace;
}

void Player::DisplayInfo() {

}