#include "./Player.h"
#include <limits>
#include <chrono>
#include "../Strategist/Strategist.h"

void Player::onStart(BWAPI::Race race) {
	this->armyUnits.clear();
	this->nonArmyUnits.clear();
	this->buildingUnits.clear();
	this->allUnits.clear();
	this->playerRace = race;
}

void Player::onFrame() {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (auto& [key, value] : this->armyUnits) {
		value->onFrame();
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	if (!armyUnits.empty()) {
		BWAPI::Broodwar->drawTextScreen(0, 60, "Army time: %d ms", duration);
		BWAPI::Broodwar->drawTextScreen(120, 60, "Per unit: %d ms", (duration / armyUnits.size()));
	}

	t1 = std::chrono::high_resolution_clock::now();
	for (auto& [key, value] : this->nonArmyUnits) {
		value->onFrame();
	}
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	if (!nonArmyUnits.empty()) {
		BWAPI::Broodwar->drawTextScreen(0, 70, "NonArmy time: %d ms", duration);
		BWAPI::Broodwar->drawTextScreen(120, 70, "Per unit: %d ms", (duration / nonArmyUnits.size()));
	}

	t1 = std::chrono::high_resolution_clock::now();
	for (auto& [key, value] : this->buildingUnits) {
		value->onFrame();
	}
	t2 = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	if (!buildingUnits.empty()) {
		BWAPI::Broodwar->drawTextScreen(0, 80, "Building time: %d ms", duration);
		BWAPI::Broodwar->drawTextScreen(120, 80, "Per unit: %d ms", (duration / buildingUnits.size()));
	}
}

void Player::onNukeDetect(BWAPI::Position target) {
}

void Player::onUnitEvade(BWAPI::Unit unit) {
}

void Player::onUnitHide(BWAPI::Unit unit) {
}

void Player::onUnitCreate(BWAPI::Unit unit) {
	//If this is the first time seeing an enemy unit, we now know what race the enemy is
	if (this->playerRace == BWAPI::Races::Unknown) {
		this->playerRace = unit->getType().getRace();
		Strategist::getInstance().swapBuildOrder();
	}

	if (unit->getType().isBuilding()) {
		auto area = BWEM::Map::Instance().GetArea(unit->getTilePosition());
		if (area) {
			if (buildingAreas.insert(area).second) {
				for (auto& mineral : area->Minerals()) {
					allMinerals[mineral] = 0;
				}
				for (auto& geyser : area->Geysers()) {
					allGeysers[geyser] = 0;
				}
			}
		}
		this->buildingUnits[unit->getID()] = std::make_unique<BuildingWrapper>(BuildingWrapper(unit));
	}
	else if (unit->getType().isWorker()) {
		this->nonArmyUnits[unit->getID()] = std::make_unique<WorkerWrapper>(WorkerWrapper(unit));
	}
	else {
		switch (unit->getType()) {
		case BWAPI::UnitTypes::Zerg_Larva: this->nonArmyUnits[unit->getID()] = std::make_shared<LarvaWrapper>(LarvaWrapper(unit)); break;
		case BWAPI::UnitTypes::Zerg_Overlord: this->nonArmyUnits[unit->getID()] = std::make_shared<OverlordWrapper>(OverlordWrapper(unit)); break;
		case BWAPI::UnitTypes::Zerg_Zergling: this->armyUnits[unit->getID()] = std::make_shared<ZerglingWrapper>(ZerglingWrapper(unit)); break;
		case BWAPI::UnitTypes::Zerg_Hydralisk: this->armyUnits[unit->getID()] = std::make_shared<HydraliskWrapper>(HydraliskWrapper(unit)); break;
		case BWAPI::UnitTypes::Zerg_Lurker: this->armyUnits[unit->getID()] = std::make_shared<LurkerWrapper>(LurkerWrapper(unit)); break;
		case BWAPI::UnitTypes::Zerg_Mutalisk: this->armyUnits[unit->getID()] = std::make_shared<MutaliskWrapper>(MutaliskWrapper(unit)); break;
		default: this->armyUnits[unit->getID()] = std::make_unique<ArmyWrapper>(ArmyWrapper(unit)); break;
		}
	}
	this->allUnits[unit->getID()] = std::make_unique<UnitWrapper>(UnitWrapper(unit));
}

void Player::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getType().isBuilding()) {
		auto wrapper = this->buildingUnits.find(unit->getID());
		if (wrapper != this->buildingUnits.end()) {
			bool areaStillOwned = false;
			auto area = BWEM::Map::Instance().GetArea(wrapper->second->getTilePosition());
			for (const auto& building : this->buildingUnits) {
				// ignore the building being destroyed
				if (building.first == wrapper->first) continue;
				if (BWEM::Map::Instance().GetArea(building.second->getTilePosition()) == area) {
					areaStillOwned = true;
				}
			}
			if (!areaStillOwned) {
				this->buildingAreas.erase(area);
			}
		}

		this->buildingUnits.erase(unit->getID());
	} else if (unit->getType().isWorker()) {
		auto unitIter = this->nonArmyUnits.find(unit->getID());
		if(unitIter != nonArmyUnits.end()){
			auto worker = static_cast<WorkerWrapper*>(unitIter->second.get());
			auto res = worker->getCurrentResource();
			if(res){
				this->adjustResourceWorkerCount(res, -1);
			}
		}
		this->nonArmyUnits.erase(unit->getID());

	} else if (unit->getType() == BWAPI::UnitTypes::Zerg_Larva || unit->getType() == BWAPI::UnitTypes::Zerg_Overlord) {
		this->nonArmyUnits.erase(unit->getID());
	} else {
		this->armyUnits.erase(unit->getID());
	}
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

std::unordered_map<int, std::shared_ptr<UnitWrapper>> Player::getUnitsByPredicate(std::function <bool(const std::shared_ptr<UnitWrapper>)> predicate) {
	std::unordered_map<int, std::shared_ptr<UnitWrapper>> units;
	for (const auto& [key, value] : this->allUnits) {
		if (predicate(value)) {
			units[key] = value;
		}
	}
	return units;
}

// Displays player info (race, # of units, # of buildings)
void Player::displayInfo(int x) {
	std::string race;

	switch (this->playerRace) {
		case BWAPI::Races::Enum::Protoss: race = "Protoss"; break;
		case BWAPI::Races::Enum::Terran: race = "Terran"; break;
		case BWAPI::Races::Enum::Zerg: race = "Zerg"; break;
		default: race = "Unknown"; break;
	}

	BWAPI::Broodwar->drawTextScreen(x, 50, "Race: %s", race.c_str());
	BWAPI::Broodwar->drawTextScreen(x, 60, "Units: %d", this->armyUnits.size() + this->nonArmyUnits.size());
	BWAPI::Broodwar->drawTextScreen(x, 70, "Buildings: %d", this->buildingUnits.size());
	BWAPI::Broodwar->drawTextScreen(x, 80, "Areas: %d", this->buildingAreas.size());

	for (auto& [key, value] : this->armyUnits) {
		value->displayInfo();
	}
	for (auto& [key, value] : this->nonArmyUnits) {
		value->displayInfo();
	}
	for (auto& [key, value] : this->buildingUnits) {
		value->displayInfo();
	}
}

void Player::updateUnitPositions() {
	for (auto& [key, value] : this->armyUnits) {
		value->updatePosition();
	}

	for (auto& [key, value] : this->nonArmyUnits) {
		value->updatePosition();
	}

	for (auto& [key, value] : this->buildingUnits) {
		value->updatePosition();
	}

	for (auto& [key, value] : this->allUnits) {
		value->updatePosition();
	}
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

std::map<BWAPI::UnitType, int> Player::getUnitCount() {
	std::map<BWAPI::UnitType, int> counts;
	for (const auto& [key, value] : this->allUnits) {
		auto countIter = counts.find(value->getUnitType());
		if (countIter != counts.end())
			countIter->second++;
		else
			counts[value->getUnitType()] = 1;
	}
	return counts;
}

BWEM::Ressource* Player::getClosestGeyser(BWAPI::Position pos) {
	return this->getClosestResource(pos, this->allGeysers);
}

BWEM::Ressource* Player::getClosestMineral(BWAPI::Position pos) {
	return this->getClosestResource(pos, this->allMinerals);
}

// We iterate through both the known geysers and the known minerals.
// sum up the amount of workers on each resource.
// and return either their ratio of gas:minerals or FLOAT_MAX if there are no mineral workers
float Player::getGasToMineralWorkerRatio() {
	int totalGeyserWorkers = 0;
	int totalMineralWorkers = 0;
	for (auto& [key, value] : this->allGeysers) {
		totalGeyserWorkers += value;
	}
	for (auto& [key, value] : this->allMinerals) {
		totalMineralWorkers += value;
	}
	return totalMineralWorkers != 0 ? (float)totalGeyserWorkers / (float)totalMineralWorkers : std::numeric_limits<float>::max();
}

// We determine if a given resource is a mineralfield or a geyser.
// If we can find the resource in our resource maps, adjust the worker count for that resource
void Player::adjustResourceWorkerCount(BWEM::Ressource* res, int val) {
	if (res->Unit()->getType().isMineralField()) {
		auto mineralIter = allMinerals.find(res);
		if(mineralIter != allMinerals.end()) {
			mineralIter->second += val;
		}
	} else {
		auto geyserIter = allGeysers.find(res);
		if(geyserIter != allGeysers.end()) {
			geyserIter->second += val;
		}
	}
}

BWEM::Ressource* Player::getClosestResource(BWAPI::Position pos, const std::map<BWEM::Ressource*, int>& resources) {
	BWEM::Ressource* closest = nullptr;
	size_t closestDistance = SIZE_MAX;

	for (auto& [key, value] : resources) {
		if (value >= 3)
			continue;
		auto path = BWEB::Path(pos, key->Pos(), BWAPI::UnitTypes::Zerg_Drone);
		path.generateJPS([&](const BWAPI::TilePosition& pos) {
			BWAPI::WalkPosition walkPos = BWAPI::WalkPosition(pos);
			for (int i = 0; i < (BWAPI::TILEPOSITION_SCALE / BWAPI::WALKPOSITION_SCALE); i++) {
				walkPos = BWAPI::WalkPosition(walkPos.x + (i % 2), walkPos.y + (i / 2));
				if (!BWAPI::Broodwar->isWalkable(walkPos))
					return false;
			}
			 return true;
		});
		if(path.isReachable() && path.getTiles().size() < closestDistance) {
			closest = key;
			closestDistance = path.getTiles().size();
		}
	}
	return closest;
}

namespace PlayerUpgrades {
	std::map<BWAPI::Player, std::shared_ptr<Upgrades>> playerUpgradesMap;

	void onStart() {
		playerUpgradesMap.clear();
	}

	void onFrame() {
		for (const auto& playerUpgrades : playerUpgradesMap)
			playerUpgrades.second.get()->onFrame();
	}

	std::shared_ptr<Upgrades> getPlayerUpgrades(BWAPI::Player player) {
		auto upgradesIter = playerUpgradesMap.find(player);
		if (upgradesIter != playerUpgradesMap.end())
			return upgradesIter->second;
		auto upgrades = std::make_shared<Upgrades>(player);
		playerUpgradesMap[player] = upgrades;
		return upgrades;
	}
};