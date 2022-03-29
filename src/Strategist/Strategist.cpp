#include "Strategist.h"
#include "../Players/Player.h"
#include "./BuildOrders/UnknownBuildOrders.h"
#include "./BuildOrders/ZergBuildOrders.h"
#include "./BuildOrders/TerranBuildOrders.h"
#include "./BuildOrders/ProtossBuildOrders.h"

void Strategist::onStart() {
    // Get initial gamestate information
    supply_total = BWAPI::Broodwar->self()->supplyTotal();
    chooseOpeningBuildOrder();
    playDecision = PlayDecision::scout;
}

void Strategist::onFrame() {
    // Check if we need to add something to the queue
    if (!build_order_queue.empty()) {
        updateUnitQueue();
    }

    if (playDecision == PlayDecision::scout && foundEnemyBase()) {
        playDecision = PlayDecision::none;
    }
}

void Strategist::incrementSupply() {
    supply_total += 16; // supply provided by an overlord
}

void Strategist::decrementSupply() {
    supply_total -= 16; // supply provided by an overlord, should trigger on overlord death.
}

void Strategist::determineMapSize() {
    // For now, we determine map size based off # of spawn locations
    if (BWAPI::Broodwar->getStartLocations().size() <= 4) {
        map_size = MapSize::smallest;
    }
    else if (BWAPI::Broodwar->getStartLocations().size() <= 6) {
        map_size = MapSize::medium;
    }
    else {
        map_size = MapSize::large;
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

    while (!build_order_queue.empty()) {
        // Check if UnitType we are checking for is valid / has been built
        if (unitsByCount.count(build_order_queue.front().first) > 0) {
            if (unitsByCount[(build_order_queue.front().first)] > 0) {
                // Remove from build order queue and decrement counter
                build_order_queue.pop();
                unitsByCount[(build_order_queue.front().first)]--;
            }
            else {
                newBuildQueue.push(build_order_queue.front());
                build_order_queue.pop();
            }
        }
        else {
            newBuildQueue.push(build_order_queue.front());
            build_order_queue.pop();
        }
    }

    build_order_queue.swap(newBuildQueue);
}

std::optional<BWAPI::UnitType> Strategist::getUnitOrder(BWAPI::UnitType type) {
    std::optional<BWAPI::UnitType> unit = std::nullopt;

    switch (type) {
    case BWAPI::UnitTypes::Zerg_Larva: 
        if (!larva_queue.empty()) {
            unit = larva_queue.front(); 
            larva_queue.pop();
        }
        break;
    case BWAPI::UnitTypes::Zerg_Drone:
        if (!drone_queue.empty()) {
            unit = drone_queue.front();
            drone_queue.pop();
        }
        break;
    case BWAPI::UnitTypes::Zerg_Hatchery: 
        if (!hatchery_queue.empty()) {
            unit = hatchery_queue.front(); 
            hatchery_queue.pop();
        }
        break;
    }
    return unit;
}

void Strategist::chooseOpeningBuildOrder() {

    // BuildOrder lists are named with the convention
    // race_mapsize
    // example: protoss_smallest

    switch (Player::getEnemyInstance().getRace()) {
    case BWAPI::Races::Protoss:
        switch (map_size) {
        case MapSize::smallest: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_smallest); break;
        case MapSize::medium: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_medium); break;
        case MapSize::large: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_large); break;
        }
    case BWAPI::Races::Terran: 
        switch (map_size) {
        case MapSize::smallest: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_smallest); break;
        case MapSize::medium: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_medium); break;
        case MapSize::large: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_large); break;
        }
    case BWAPI::Races::Zerg:
        switch (map_size) {
        case MapSize::smallest: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_smallest); break;
        case MapSize::medium: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_medium); break;
        case MapSize::large: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_large); break;
        }
    default:
        switch (map_size) {
        case MapSize::smallest: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_smallest); break;
        case MapSize::medium: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_medium); break;
        case MapSize::large: build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_large); break;
        }
    }
}

void Strategist::updateUnitQueue() {
    // Need to track the amount of gas / minerals spent each frame in order to be able to queue multiple units on a single frame
    int frame_supply_used = 0;

    while(!build_order_queue.empty() && 
        ((BWAPI::Broodwar->self()->gatheredGas() - this->gas_spent) >= build_order_queue.front().first.gasPrice()) && 
        ((BWAPI::Broodwar->self()->gatheredMinerals() - this->minerals_spent) >= build_order_queue.front().first.mineralPrice()) &&
        ((BWAPI::Broodwar->self()->supplyUsed() + frame_supply_used) >= build_order_queue.front().second)) {
        // Sufficient minerals, gas, and proper supply to build the unit

        switch(build_order_queue.front().first.whatBuilds().first) {
            case BWAPI::UnitTypes::Zerg_Larva: larva_queue.push(build_order_queue.front().first); break;
            case BWAPI::UnitTypes::Zerg_Drone: drone_queue.push(build_order_queue.front().first); break;
            case BWAPI::UnitTypes::Zerg_Hatchery: hatchery_queue.push(build_order_queue.front().first); break;
        }
        if (build_order_queue.front().first.supplyProvided() > 0) {
            incrementSupply();
        }
        // update spent minerals + gas + supply
        minerals_spent += build_order_queue.front().first.mineralPrice();
        gas_spent += build_order_queue.front().first.gasPrice();

        frame_supply_used += (build_order_queue.front().first == BWAPI::UnitTypes::Zerg_Zergling) ? (build_order_queue.front().first.supplyRequired() * 2) : build_order_queue.front().first.supplyRequired();

        // Pop from build_order_queue
        build_order_queue.pop();
    }
}

bool Strategist::foundEnemyBase() {
    std::unordered_map<int, BWAPI::Unit> depot = Player::getEnemyInstance().getUnitsByType(Player::getEnemyInstance().getRace().getResourceDepot());
    return(!depot.empty());
}

void Strategist::displayInfo(int x) {
    std::string play = "";
    switch (playDecision) {
        case PlayDecision::scout: play = "Scout"; break;
        case PlayDecision::attack: play = "Attack"; break;
        case PlayDecision::defend: play = "Defend"; break;
        default: play = "None"; break;
    }
    BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Large);
    BWAPI::Broodwar->drawTextScreen(x, 95, "Play: %s", play.c_str());
    BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Default);
}