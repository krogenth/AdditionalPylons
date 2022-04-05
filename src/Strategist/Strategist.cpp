#include "Strategist.h"

#include "./BuildOrders/UnknownBuildOrders.h"
#include "./BuildOrders/ZergBuildOrders.h"
#include "./BuildOrders/TerranBuildOrders.h"
#include "./BuildOrders/ProtossBuildOrders.h"

#include "../Players/Player.h"
#include "../Lambdas/Unit/UnitLambdas.h"

void Strategist::onStart() {
    // Get initial gamestate information
    this->totalSupply = BWAPI::Broodwar->self()->supplyTotal();
    this->chooseOpeningBuildOrder();
}

void Strategist::onFrame() {
    // Check if we need to add something to the queue
    if (!this->startingBuildQueue.empty()) {
        this->updateUnitQueue();
    }

    if (!this->checkIfEnemyFound()) {
        this->playDecision = PlayDecision::scout;
    } else {
        this->playDecision = PlayDecision::attack;
    }
}

void Strategist::determineMapSize() {
    // For now, we determine map size based off # of spawn locations
    if (BWAPI::Broodwar->getStartLocations().size() <= 4) {
        this->mapSize = MapSize::smallest;
    }
    else if (BWAPI::Broodwar->getStartLocations().size() <= 6) {
        this->mapSize = MapSize::medium;
    }
    else {
        this->mapSize = MapSize::large;
    }
}

void Strategist::swapBuildOrder() {
    // Note: swapBuildOrder() function should only be called when we first discover enemy race

    std::queue<std::pair<BWAPI::UnitType, int>> newBuildQueue;
    auto unitsByCount = Player::getPlayerInstance().getUnitCount();

    // Adjust for starting units
    unitsByCount[BWAPI::UnitTypes::Zerg_Drone] -= 4;
    unitsByCount[BWAPI::UnitTypes::Zerg_Hatchery] -= 1;

    // Adjust for drones morphed into buildings
    for (auto& [key, value] : unitsByCount) {
        if (key.isBuilding()) {
            unitsByCount[BWAPI::UnitTypes::Zerg_Drone] += value;
        }
    }

    // Update build_order_queue and begin comparison to what we have
    this->chooseOpeningBuildOrder();

    while (!this->startingBuildQueue.empty()) {
        // check that BWAPI::UnitType exists, and the count is above 0
        if (unitsByCount.find(this->startingBuildQueue.front().first) != unitsByCount.end() &&
            unitsByCount[this->startingBuildQueue.front().first] > 0) {
            unitsByCount[this->startingBuildQueue.front().first]--;
        }
        // else, add it to the new queue
        else {
            newBuildQueue.push(this->startingBuildQueue.front());
        }
        this->startingBuildQueue.pop();
    }

    this->startingBuildQueue.swap(newBuildQueue);
}

std::optional<BWAPI::UnitType> Strategist::getUnitOrder(BWAPI::UnitType type) {
    std::optional<BWAPI::UnitType> unit = std::nullopt;

    auto buildIter = this->buildOrders.find(type);
    if (buildIter != this->buildOrders.end()) {
        if (buildIter->second.size()) {
            unit = buildIter->second.front();
            buildIter->second.pop();
        }
    }
    return unit;
}

std::optional<BWAPI::UpgradeType> Strategist::getUnitUpgradeOrder(BWAPI::UnitType type) {
    std::optional<BWAPI::UpgradeType> upgrade = std::nullopt;

    auto upgradeIter = this->upgradeOrders.find(type);
    if (upgradeIter != this->upgradeOrders.end()) {
        if (upgradeIter->second.size()) {
            upgrade = upgradeIter->second.front();
            upgradeIter->second.pop();
        }
    }
    return upgrade;
}

void Strategist::chooseOpeningBuildOrder() {

    // BuildOrder lists are named with the convention
    // race_mapsize
    // example: protoss_smallest

    switch (Player::getEnemyInstance().getRace()) {
    case BWAPI::Races::Protoss:
        switch (this->mapSize) {
        case MapSize::smallest: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_smallest); break;
        case MapSize::medium: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_medium); break;
        case MapSize::large: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_large); break;
        }
    case BWAPI::Races::Terran:
        switch (this->mapSize) {
        case MapSize::smallest: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_smallest); break;
        case MapSize::medium: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_medium); break;
        case MapSize::large: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_large); break;
        }
    case BWAPI::Races::Zerg:
        switch (this->mapSize) {
        case MapSize::smallest: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_smallest); break;
        case MapSize::medium: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_medium); break;
        case MapSize::large: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_large); break;
        }
    default:
        switch (this->mapSize) {
        case MapSize::smallest: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_smallest); break;
        case MapSize::medium: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_medium); break;
        case MapSize::large: this->startingBuildQueue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_large); break;
        }
    }
}

void Strategist::updateUnitQueue() {
    // Need to track the amount of gas / minerals spent each frame in order to be able to queue multiple units on a single frame
    int frame_supply_used = 0;

    // Sufficient minerals, gas, and proper supply to build the unit
    while(!this->startingBuildQueue.empty() &&
        ((BWAPI::Broodwar->self()->gatheredGas() - this->spentGas) >= this->startingBuildQueue.front().first.gasPrice()) &&
        ((BWAPI::Broodwar->self()->gatheredMinerals() - this->spentMinerals) >= this->startingBuildQueue.front().first.mineralPrice()) &&
        ((BWAPI::Broodwar->self()->supplyUsed() + frame_supply_used) >= this->startingBuildQueue.front().second)) {
        
        if (this->buildOrders.find(this->startingBuildQueue.front().first.whatBuilds().first) == this->buildOrders.end())
            this->buildOrders[this->startingBuildQueue.front().first.whatBuilds().first] = std::queue<BWAPI::UnitType>{};
        this->buildOrders[this->startingBuildQueue.front().first.whatBuilds().first].push(this->startingBuildQueue.front().first);

        this->adjustTotalSupply(this->startingBuildQueue.front().first.supplyProvided());

        // update spent minerals + gas + supply
        this->spentMinerals += this->startingBuildQueue.front().first.mineralPrice();
        this->spentGas += this->startingBuildQueue.front().first.gasPrice();

        frame_supply_used += (this->startingBuildQueue.front().first == BWAPI::UnitTypes::Zerg_Zergling) ? (this->startingBuildQueue.front().first.supplyRequired() * 2) : this->startingBuildQueue.front().first.supplyRequired();

        // Pop from build_order_queue
        this->startingBuildQueue.pop();
    }
}

bool Strategist::checkIfEnemyFound() {
    return(!(Player::getEnemyInstance().getUnitsByPredicate(lambdas::unit::getBuildingsLambda).empty() &&
        Player::getEnemyInstance().getUnitsByPredicate(lambdas::unit::getVisibleArmyUnitsLambda).empty()));
}

void Strategist::displayInfo(int x) {
    std::string play = "";
    switch (playDecision) {
        case PlayDecision::scout: play = "Scout"; break;
        case PlayDecision::attack: play = "Attack"; break;
        case PlayDecision::defend: play = "Defend"; break;
        default: play = "None"; break;
    }
    BWAPI::Broodwar->drawTextScreen(x, 80, "Play: %s", play.c_str());
}