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
    // Check if we have the resources to build + correct supply count
    bool gasRequired = BWAPI::Broodwar->self()->gas() >= build_order_queue.front().first.gasPrice();
    bool mineralsRequired = BWAPI::Broodwar->self()->minerals() >= build_order_queue.front().first.mineralPrice();
    bool supplyAvailable = ((this->supply_total - BWAPI::Broodwar->self()->supplyUsed()) > 0);

    if (gasRequired && mineralsRequired && supplyAvailable) {
        switch(build_order_queue.front().first.whatBuilds().first){
            case BWAPI::UnitTypes::Zerg_Larva: larva_queue.push(build_order_queue.front().first); break;
            case BWAPI::UnitTypes::Zerg_Drone: drone_queue.push(build_order_queue.front().first); break;
            case BWAPI::UnitTypes::Zerg_Hatchery: hatchery_queue.push(build_order_queue.front().first); break;
        }
        if (build_order_queue.front().first.supplyProvided() > 0){
            incrementSupply();
        }
        // update spent minerals
        minerals_spent += build_order_queue.front().first.mineralPrice();

        // Pop from build_order_queue
        build_order_queue.pop();
    }
}