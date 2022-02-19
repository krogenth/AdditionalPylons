#include "Strategist.h"
#include "./BuildOrders/UnknownBuildOrders.h"
#include "./BuildOrders/ZergBuildOrders.h"
#include "./BuildOrders/TerranBuildOrders.h"
#include "./BuildOrders/ProtossBuildOrders.h"

void Strategist::onStart() {
    // Get initial gamestate information
    supply_total = BWAPI::Broodwar->self()->supplyTotal();
    enemy_race = BWAPI::Broodwar->enemy()->getRace(); // Try to get enemy race on startup.  If the enemy chose random race, unknown will be returned.
    chooseOpeningBuildOrder();
}

void Strategist::onFrame(){
    // Check if we need to add something to the queue
    if (build_order_queue.empty() == false) {
        updateUnitQueue();
    }
}

void Strategist::incrementSupply(){
    supply_total += 16; // supply provided by an overlord
}

void Strategist::decrementSupply(){
    supply_total -= 16;// supply provided by an overlord, should trigger on overlord death.
}

void Strategist::determineMapSize(){
    // Need to figure out how we want to determine this, or if we even want to determine it in the strategist.
    map_size = smallest;
}

std::optional<BWAPI::UnitType> Strategist::getUnitOrder(BWAPI::UnitType type){
    std::optional<BWAPI::UnitType> unit = std::nullopt;

    switch (type.whatBuilds().first) {
    case BWAPI::UnitTypes::Zerg_Larva: unit = larva_queue.front(); larva_queue.pop();  break;
    case BWAPI::UnitTypes::Zerg_Drone: unit = drone_queue.front(); drone_queue.pop(); break;
    case BWAPI::UnitTypes::Zerg_Hatchery: unit = hatchery_queue.front(); hatchery_queue.pop(); break;
    }
    
    return unit;
}

void Strategist::chooseOpeningBuildOrder(){

    // BuildOrder lists are named with the convention
    // race_mapsize
    // example: protoss_smallest

    // Check by race first
    if (enemy_race == BWAPI::Races::Protoss) {
        // Check map size
        if (map_size == smallest) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_smallest);
        }
        else if (map_size == medium) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_medium);
        }
        else {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(protoss_large);
        }
    }
    else if (enemy_race == BWAPI::Races::Terran) {
        if (map_size == smallest) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_smallest);
        }
        else if (map_size == medium) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_medium);
        }
        else {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(terran_large);
        }
    }
    else if (enemy_race == BWAPI::Races::Zerg) {
        if (map_size == smallest) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_smallest);
        }
        else if (map_size == medium) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_medium);
        }
        else {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(zerg_large);
        }
    }
    else { // Enemy race is unknown to us
        if (map_size == smallest) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_smallest);
        }
        else if (map_size == medium) {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_medium);
        }
        else {
            build_order_queue = std::queue<std::pair<BWAPI::UnitType, int>>(unknown_large);
        }
    }
}

void Strategist::updateUnitQueue(){
    // Need to track the amount of gas / minerals spent each frame in order to be able to queue multiple units on a single frame
    int frame_minerals_spent = 0;
    int frame_gas_spent = 0;
    int frame_supply_used = 0;

    // Check if we have the resources to build + correct supply count
    bool gasRequired = BWAPI::Broodwar->self()->gas() >= build_order_queue.front().first.gasPrice();
    bool mineralsRequired = BWAPI::Broodwar->self()->minerals() >= build_order_queue.front().first.mineralPrice();
    bool supplyAvailable = BWAPI::Broodwar->self()->supplyUsed() >= build_order_queue.front().second;

    while (gasRequired && mineralsRequired && supplyAvailable) {
        switch(build_order_queue.front().first.whatBuilds().first){
            case BWAPI::UnitTypes::Zerg_Larva: larva_queue.push(build_order_queue.front().first); break;
            case BWAPI::UnitTypes::Zerg_Drone: drone_queue.push(build_order_queue.front().first); break;
            case BWAPI::UnitTypes::Zerg_Hatchery: hatchery_queue.push(build_order_queue.front().first); break;
        }
        if (build_order_queue.front().first.supplyProvided() > 0){
            incrementSupply();
        }
        // update spent minerals + gas + supply
        minerals_spent += build_order_queue.front().first.mineralPrice();
        frame_minerals_spent += build_order_queue.front().first.mineralPrice();

        gas_spent += build_order_queue.front().first.gasPrice();
        frame_gas_spent += build_order_queue.front().first.gasPrice();

        frame_supply_used += (build_order_queue.front().first == BWAPI::UnitTypes::Zerg_Zergling) ? (build_order_queue.front().first.supplyRequired() * 2) : build_order_queue.front().first.supplyRequired();

        // Pop from build_order_queue
        build_order_queue.pop();

        // check to see if we can build the next unit in queue
        gasRequired = (BWAPI::Broodwar->self()->gas() - frame_gas_spent) >= build_order_queue.front().first.gasPrice();
        mineralsRequired = (BWAPI::Broodwar->self()->minerals() - frame_minerals_spent) >= build_order_queue.front().first.mineralPrice();
        supplyAvailable = (BWAPI::Broodwar->self()->supplyUsed() + frame_supply_used) >= build_order_queue.front().second;
    }
}