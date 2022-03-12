#pragma once
#include <BWAPI.h>
#include <queue>
#include <optional>

enum class MapSize { smallest, medium, large };
enum class PlayDecision { none, scout, attack, defend };
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
    void displayInfo(int x);
    /*
    Returns the next build order by the requesters unit type, if there is one
    @returns
        @retval std::optional<BWAPI::UnitType>
    */
    std::optional<BWAPI::UnitType> getUnitOrder(BWAPI::UnitType type);
    
        
private:
    Strategist() = default;

    void determineMapSize();
    void chooseOpeningBuildOrder();
    void updateUnitQueue();
    /*
    Returns if we have found an enemy resource depot
    @returns
        @retval bool true if map is not empty
    */
    bool foundEnemyBase();

    int minerals_spent = 0;
    int gas_spent = 0;
    int supply_total;
  
    std::queue<BWAPI::UnitType> larva_queue;
    std::queue<BWAPI::UnitType> drone_queue;
    std::queue<BWAPI::UnitType> hatchery_queue;

    MapSize map_size;
    PlayDecision playDecision;

    std::queue<std::pair<BWAPI::UnitType, int>> build_order_queue;
};