#include "./Player.h"
#include <limits>
// Auto assigns this player's race and sets enemies race to unknown
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

// When a unit is created, adds unit to maps corresponding to type
void Player::onUnitCreate(BWAPI::Unit unit) {
    // If this is the first time seeing an enemy unit, we now know what race the enemy is
    if (this->playerRace == BWAPI::Races::Unknown)
        this->playerRace = unit->getType().getRace();

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
    } else if (unit->getType().isWorker()) {
        this->nonArmyUnits[unit->getID()] = std::make_unique<WorkerWrapper>(WorkerWrapper(unit));
    } else {
        switch (unit->getType()) {
            case BWAPI::UnitTypes::Zerg_Larva:
                this->nonArmyUnits[unit->getID()] = std::make_unique<LarvaWrapper>(LarvaWrapper(unit));
                break;
            case BWAPI::UnitTypes::Zerg_Overlord:
                this->nonArmyUnits[unit->getID()] = std::make_unique<OverlordWrapper>(OverlordWrapper(unit));
                break;
            case BWAPI::UnitTypes::Zerg_Zergling:
                this->armyUnits[unit->getID()] = std::make_unique<ZerglingWrapper>(ZerglingWrapper(unit));
                break;
            case BWAPI::UnitTypes::Zerg_Hydralisk:
                this->armyUnits[unit->getID()] = std::make_unique<HydraliskWrapper>(HydraliskWrapper(unit));
                break;
            case BWAPI::UnitTypes::Zerg_Lurker:
                this->armyUnits[unit->getID()] = std::make_unique<LurkerWrapper>(LurkerWrapper(unit));
                break;
            case BWAPI::UnitTypes::Zerg_Mutalisk:
                this->armyUnits[unit->getID()] = std::make_unique<MutaliskWrapper>(MutaliskWrapper(unit));
                break;
            default:
                this->armyUnits[unit->getID()] = std::make_unique<ArmyWrapper>(ArmyWrapper(unit));
                break;
        }
    }
    this->allUnits[unit->getID()] = std::make_unique<UnitWrapper>(UnitWrapper(unit));
}

// When a unit is destroyed, removes unit from maps corresponding type
void Player::onUnitDestroy(BWAPI::Unit unit) {

    if (unit->getType().isBuilding()) {
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

std::unordered_map<int, BWAPI::Unit> Player::getUnitsByType(BWAPI::UnitType type) {
    std::unordered_map<int, BWAPI::Unit> specUnits;
    if (type == BWAPI::UnitTypes::Unknown) {
        for (auto& [key, value] : this->allUnits) {
            specUnits[value->getID()] = value->getUnit();
        }
    } else {
        for (auto& [key, value] : this->allUnits) {
            if (value->getUnitType() == type)
                specUnits[value->getID()] = value->getUnit();
        }
    }

    return specUnits;
}

// Displays player info (race, # of units, # of buildings)
void Player::displayInfo(int x) {
    std::string race;

    switch (this->playerRace) {
        case BWAPI::Races::Enum::Protoss:
            race = "Protoss";
            break;
        case BWAPI::Races::Enum::Terran:
            race = "Terran";
            break;
        case BWAPI::Races::Enum::Zerg:
            race = "Zerg";
            break;
        default:
            race = "Unknown";
            break;
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

BWEM::Ressource* getClosestResource(BWAPI::Position pos, const std::map<BWEM::Ressource*, int>& resources) {
    std::map<BWEM::Ressource*, int> resources;
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
        if(path.isReachable() && path.getTiles().size() < closestDistance){
            closest = key;
            closestDistance = path.getTiles().size();
        }
    }
    return closest;
}