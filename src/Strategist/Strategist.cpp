#include "Strategist.h"

void Strategist::onStart() {
    // Get initial gamestate information
    build_order_list = chooseOpeningBuildOrder(DetermineMapSize());
    minerals_spent = 0;
    list_pos = 0;
    supply_total = BWAPI::Broodwar->self().supply_total();
    enemy_race = BWAPI::Broodwar->enemy().getRace(); // Try to get enemy race on startup.  If the enemy chose random race, unknown will be returned.
}

void Strategist::onFrame(){
    // Check if we have the resources to build + correct supply count
    bool gasRequired = BWAPI::Broodwar->self()->gas() >= build_order_list[list_pos].first().gasPrice();
    bool mineralsRequired = BWAPI::Broodwar->self()->minerals() >= build_order_list[list_pos].first().mineralPrice();
    bool supplyAvailable = ((BWAPI::Broodwar->self()->supplyUsed() <= build_order_list[list_pos].second()) && BWAPI::Broodwar->self()->supplyUsed() + build_order_list[list_pos].first().supplyRequired() <= supply_total);

    // Add it to the proper queue if all conditions are met
    if (gasRequired && mineralsRequired && supplyAvailable) {
        // enum values found at https://bwapi.github.io/namespace_b_w_a_p_i_1_1_unit_types_1_1_enum.html
        switch(build_order_list[list_pos].first().whatBuilds().first()){
            case 0x23: larva_queue.push(build_order_list[list_pos].first()); break;
            case 0x29: drone_queue.push(build_order_list[list_pos].first()); break;
            case 0x83: hatchery_queue.push(build_order_list[list_pos].first()); break;
        }
        if (build_order_list[list_pos].first().supplyProvided() > 0){
            incrementSupply();
        }
        // update spent minerals
        minerals_spent += build_order_list[list_pos].first().mineralPrice();

        // increment list_pos
        list_pos++;
    }
}

void Strategist::incrementSupply(){
    supply_total += 16; // supply provided by an overlord
}

void Strategist::decrementSupply(){
    supply_total -= 16;// supply provided by an overlord, should trigger on overlord death.
}

MapSize Strategist::DetermineMapSize(){
    // Need to figure out how we want to determine this, or if we even want to determine it in the strategist.
    MapSize size = small;
    return size;
}
