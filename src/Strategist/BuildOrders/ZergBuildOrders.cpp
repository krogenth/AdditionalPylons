#include "ZergBuildOrders.h"

// Opt for 4 pool on small maps
// https://liquipedia.net/starcraft/4/5_Pool
std::initializer_list<std::pair<BWAPI::UnitType, int>> zerg_smallest = {
    {BWAPI::UnitTypes::Zerg_Spawning_Pool, 8},
    {BWAPI::UnitTypes::Zerg_Zergling, 6},
    {BWAPI::UnitTypes::Zerg_Zergling, 8},
    {BWAPI::UnitTypes::Zerg_Zergling, 10},
    {BWAPI::UnitTypes::Zerg_Zergling, 12},
    {BWAPI::UnitTypes::Zerg_Zergling, 14},
    {BWAPI::UnitTypes::Zerg_Zergling, 16},
    {BWAPI::UnitTypes::Zerg_Overlord, 18}
};

// 9 pool on medium maps
// https://liquipedia.net/starcraft/9_Pool_(vs._Zerg)
std::initializer_list<std::pair<BWAPI::UnitType, int>> zerg_medium = {
    {BWAPI::UnitTypes::Zerg_Drone, 8},
    {BWAPI::UnitTypes::Zerg_Drone, 10},
    {BWAPI::UnitTypes::Zerg_Drone, 12},
    {BWAPI::UnitTypes::Zerg_Drone, 14},
    {BWAPI::UnitTypes::Zerg_Drone, 16},
    {BWAPI::UnitTypes::Zerg_Spawning_Pool, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 16},
    {BWAPI::UnitTypes::Zerg_Extractor, 18},
    {BWAPI::UnitTypes::Zerg_Overlord, 16},
    {BWAPI::UnitTypes::Zerg_Zergling, 16},
    {BWAPI::UnitTypes::Zerg_Zergling, 18},
    {BWAPI::UnitTypes::Zerg_Zergling, 20}
};

// 12 hatch on large maps
// https://liquipedia.net/starcraft/12_Hatch_(vs._Zerg)
std::initializer_list<std::pair<BWAPI::UnitType, int>> zerg_large = {
    {BWAPI::UnitTypes::Zerg_Drone, 8},
    {BWAPI::UnitTypes::Zerg_Drone, 10},
    {BWAPI::UnitTypes::Zerg_Drone, 12},
    {BWAPI::UnitTypes::Zerg_Drone, 14},
    {BWAPI::UnitTypes::Zerg_Drone, 16},
    {BWAPI::UnitTypes::Zerg_Overlord, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 20},
    {BWAPI::UnitTypes::Zerg_Drone, 22},
    {BWAPI::UnitTypes::Zerg_Hatchery, 24},
    {BWAPI::UnitTypes::Zerg_Spawning_Pool, 22},
    {BWAPI::UnitTypes::Zerg_Extractor, 20},
    {BWAPI::UnitTypes::Zerg_Drone, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 20},
    {BWAPI::UnitTypes::Zerg_Drone, 22},
    {BWAPI::UnitTypes::Zerg_Zergling, 24},
    {BWAPI::UnitTypes::Zerg_Zergling, 26},
    {BWAPI::UnitTypes::Zerg_Zergling, 28},
};