#include "Strategist.h"
#include "../Players/Player.h"
#include "./BuildOrders/UnknownBuildOrders.h"
#include "./BuildOrders/ZergBuildOrders.h"
#include "./BuildOrders/TerranBuildOrders.h"
#include "./BuildOrders/ProtossBuildOrders.h"

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
}

void Strategist::determineMapSize() {
    // Need to figure out how we want to determine this, or if we even want to determine it in the strategist.
    this->mapSize = MapSize::smallest;
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

    switch (enemy.getRace()) {
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