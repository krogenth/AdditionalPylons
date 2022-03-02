#include "./Player.h"

//Auto assigns this player's race and sets enemies race to unknown
void Player::onStart(BWAPI::Race race) {
	this->armyUnits.clear();
	this->nonArmyUnits.clear();
	this->buildingUnits.clear();
	this->allUnits.clear();
	this->playerRace = race;
}

void Player::onFrame() {
	for (auto& [key, value] : this->armyUnits) {
		value->onFrame();
		value->displayInfo();
	}
		
	for (auto& [key, value] : this->nonArmyUnits) {
		value->onFrame();
		value->displayInfo();
	}
		
	for (auto& [key, value] : this->buildingUnits) {
		value->onFrame();
		value->displayInfo();
	}
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

	if (unit->getType().isBuilding())
		this->buildingUnits[unit->getID()] = std::make_unique<BuildingWrapper>(BuildingWrapper(unit));
	else if (unit->getType().isWorker())
		this->nonArmyUnits[unit->getID()] = std::make_unique<WorkerWrapper>(WorkerWrapper(unit));
	else if (unit->getType() == BWAPI::UnitTypes::Zerg_Larva)
		this->nonArmyUnits[unit->getID()] = std::make_unique<LarvaWrapper>(LarvaWrapper(unit));
	else if (unit->getType() == BWAPI::UnitTypes::Zerg_Overlord)
		this->nonArmyUnits[unit->getID()] = std::make_unique<OverlordWrapper>(OverlordWrapper(unit));
	else
		this->armyUnits[unit->getID()] = std::make_unique<ArmyWrapper>(ArmyWrapper(unit));
	this->allUnits[unit->getID()] = std::make_unique<UnitWrapper>(UnitWrapper(unit));
}

//When a unit is destroyed, removes unit from maps corresponding type
void Player::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getType().isBuilding())
		this->buildingUnits.erase(unit->getID());
	else if (unit->getType().isWorker() || unit->getType() == BWAPI::UnitTypes::Zerg_Larva || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord)
		this->nonArmyUnits.erase(unit->getID());
	else
		this->armyUnits.erase(unit->getID());
	this->allUnits.erase(unit->getID());
}

void Player::onUnitMorph(BWAPI::Unit unit) {
	this->buildingUnits.erase(unit->getID());
	this->nonArmyUnits.erase(unit->getID());
	this->armyUnits.erase(unit->getID());
	this->allUnits.erase(unit->getID());
	onUnitCreate(unit);
}

void Player::onUnitRenegade(BWAPI::Unit unit) {

}

void Player::onUnitComplete(BWAPI::Unit unit) {

}

void Player::onUnitDiscover(BWAPI::Unit unit) {

}

std::unordered_map<int, BWAPI::Unit> Player::getUnitsByType(BWAPI::UnitType type) {
	std::unordered_map<int, BWAPI::Unit> specUnits;
	if (type == BWAPI::UnitTypes::Unknown) {
		for (auto& [key, value] : this->allUnits) {
			specUnits[value->getID()] = value->getUnit();
		}
	}
	else {
		for (auto& [key, value] : this->allUnits) {
			if (value->getUnitType() == type)
				specUnits[value->getID()] = value->getUnit();
		}
	}

	return specUnits;
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
	BWAPI::Broodwar->drawTextScreen(x, 65, "Units: %d", this->armyUnits.size() + this->nonArmyUnits.size());
	BWAPI::Broodwar->drawTextScreen(x, 80, "Buildings: %d", this->buildingUnits.size());
	BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Default);
}

std::unordered_map<int, BWAPI::Unit> Player::getUnitsByArea(BWAPI::Position topLeft, BWAPI::Position botRight) {
	std::unordered_map<int, BWAPI::Unit> areaUnits;
	for (auto& [key, value] : this->allUnits) {
		BWAPI::Position unitPos = value->getUnit()->getPosition();
		if (((topLeft == unitPos) || (topLeft < unitPos)) &&
			((unitPos == botRight) || (unitPos < botRight)))
			areaUnits[value->getID()] = value->getUnit();
		else
			continue;
	}

	return areaUnits;
}