#pragma once
#include <BWAPI.h>
#include <queue>
#include <optional>

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

    void determineMapSize();

    std::optional<BWAPI::UnitType> getUnitOrder(BWAPI::UnitType type);
    
        
private:
    Strategist() = default;

    void chooseOpeningBuildOrder();
    void updateUnitQueue();

    int minerals_spent = 0;
    int gas_spent = 0;
    int supply_total;
  
    std::queue<BWAPI::UnitType> larva_queue;
    std::queue<BWAPI::UnitType> drone_queue;
    std::queue<BWAPI::UnitType> hatchery_queue;

    BWAPI::Race enemy_race;
    MapSize map_size;

    std::queue<std::pair<BWAPI::UnitType, int>> build_order_queue;
};