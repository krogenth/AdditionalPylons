#pragma once
#include <BWAPI.h>
#include <queue>
#include "./BuildOrders/DefaultBuildOrders.h"
#include "./BuildOrders/ZergBuildOrders.h"
#include "./BuildOrders/TerranBuildOrders.h"
#include "./BuildOrders/ProtossBuildOrders.h"

enum MapSize { smallest, medium, large };

class Strategist {
public:
    Strategist(const Strategist&) = delete;
    Strategist(const Strategist&&) = delete;
    Strategist operator=(const Strategist&) = delete;
    Strategist& operator=(const Strategist&&) = delete;
    static Strategist& getInstance() {
        static Strategist instance;
        return instance;
    }

    void onStart();
    void onFrame();
    void incrementSupply();
    void decrementSupply();

    void DetermineMapSize();

    
        
private:
    Strategist() = default;

    void chooseOpeningBuildOrder();
    void updateUnitQueue();

    int minerals_spent;
    int supply_total;
  
    std::queue<BWAPI::UnitType> larva_queue;
    std::queue<BWAPI::UnitType> drone_queue;
    std::queue<BWAPI::UnitType> hatchery_queue;

    BWAPI::Race enemy_race;
    MapSize map_size;

    std::initializer_list<std::pair<BWAPI::UnitType, int>> build_order_list;
    std::queue<std::pair<BWAPI::UnitType, int>> build_order_queue;
};