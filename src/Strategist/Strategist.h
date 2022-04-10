#pragma once
#include <map>
#include <queue>
#include <optional>
#include <BWAPI.h>

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
    void adjustTotalSupply(int supply) { this->totalSupply += supply; }
    void displayInfo(int x);

    /*
    Update our build order queue to better fit the enemies race once scouted.
    Should only be called if our enemies race is initially Unknown.
    @returns
        none
    */
    void swapBuildOrder();

    /*
    Returns the next build order by the requesters BWAPI::UnitType, if there is one
    @returns
        @retval std::optional<BWAPI::UnitType>
    */
    std::optional<BWAPI::UnitType> getUnitOrder(BWAPI::UnitType type);

    PlayDecision getPlayDecision() { return this->playDecision; }

    /*
    Returns the next upgrade order by the requesters BWAPI::UnitType, if there is one
    @returns
        @retval std::optional<BWAPI::UpgradeType>
    */
    std::optional<BWAPI::UpgradeType> getUnitUpgradeOrder(BWAPI::UnitType type);
        
private:
    Strategist() = default;

    void determineMapSize();
    void chooseOpeningBuildOrder();
    void updateStartingBuildingQueues();
    void UpdateInfinityBuildQueues();
    void onStartAttemptFindEnemyStartingBase();
    bool checkIfEnemyFound();

    int spentMinerals = 0;
    int spentGas = 0;
    int totalSupply = 0;

    std::map<BWAPI::UnitType, std::queue<BWAPI::UnitType>> buildOrders;
    std::map<BWAPI::UnitType, std::queue<BWAPI::UpgradeType>> upgradeOrders;
    std::map<BWAPI::UnitType, std::queue<BWAPI::TechType>> researchOrders;

    std::queue<std::pair<BWAPI::UnitType, int>> startingBuildQueue;

    MapSize mapSize = MapSize::smallest;
    PlayDecision playDecision = PlayDecision::scout;
};