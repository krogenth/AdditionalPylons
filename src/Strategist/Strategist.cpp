#include "Strategist.h"

void Strategist::onStart() {
    // Get initial gamestate information
    build_order_list = chooseOpeningBuildOrder(DetermineMapSize());
    minerals_spent = 0;
    list_pos = 0;
    supply_total = BWAPI::Broodwar->self().supply_total();
    enemy_race = BWAPI::Broodwar->enemy().getRace(); // Try to get enemy race on startup.  If the enemy chose random race, unknown will be returned.
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

void Strategist::DetermineMapSize(){
    // Need to figure out how we want to determine this, or if we even want to determine it in the strategist.
    map_size = small;
}

void Strategist::chooseOpeningBuildOrder(){
    // For now, simply populate the queue.
    for (auto it = build_order_list.begin(); it != build_order_list.end(); ++it){
        build_order_queue.push(*it);
    }
}

void Strategist::updateUnitQueue(){
    // Check if we have the resources to build + correct supply count
    bool gasRequired = BWAPI::Broodwar->self()->gas() >= build_order_queue.front().first().gasPrice();
    bool mineralsRequired = BWAPI::Broodwar->self()->minerals() >= build_order_queue.front().first().mineralPrice();
    bool supplyAvailable = ((this->total_supply - BWAPI::Broodwar->self()->supplyUsed()) > 0);

    if (gasRequired && mineralsRequired && supplyAvailable) {
        switch(build_order_queue.front()first().whatBuilds().first()){
            case BWAPI::UnitTypes::Zerg_Larva: larva_queue.push(build_orderqueue.front().first()); break;
            case BWAPI::UnitTypes::Zerg_Drone: drone_queue.push(build_orderqueue.front().first()); break;
            case BWAPI::UnitTypes::Zerg_Hatchery: hatchery_queue.push(build_orderqueue.front().first()); break;
        }
        if (build_orderqueue.front().first().supplyProvided() > 0){
            incrementSupply();
        }
        // update spent minerals
        minerals_spent += build_orderqueue.front().first().mineralPrice();

        // Pop from build_order_queue
        build_order_queue.pop();
    }
}