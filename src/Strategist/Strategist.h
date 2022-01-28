#pragma once

enum MapSize { small, medium, large };

class Strategist {
public:
    Strategist(const Strategist&) = delete;
    Strategist(const Strategist&&) = delete;
    Strategist operator=(const Strategist&) = delete;
    Strategist& operator=(const Strategist&) = delete;
    static Strategist& getInstance() {
        static Strategist instance;
        return instance;
    }

    void onStart();
    void onFrame();
    void incrementSupply();
    void decrementSupply();

    MapSize DetermineMapSize();

    std::initializer_list<std::pair<BWAPI::UnitType, int>> chooseOpeningBuildOrder(MapSize map_size);
        
private:
    Strategist() = default;

    int minerals_spent;
    int supply_total;
  
    std::queue<BWAPI::UnitType> larva_queue;
    std::queue<BWAPI::UnitType> drone_queue;
    std::queue<BWAPI::UnitType> hatchery_queue;

    BWAPI::Race enemy_race;

    std::initializer_list<std::pair<BWAPI::UnitType, int>> build_order_list;
    int list_pos = 0;
};