#pragma once
// Opt for 4 pool on small maps
// https://liquipedia.net/starcraft/4/5_Pool
std::initializer_list<std::pair<BWAPI::UnitType, int>> terran_smallest = {
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
// https://liquipedia.net/starcraft/9_Pool_(vs._Terran)
std::initializer_list<std::pair<BWAPI::UnitType, int>> terran_medium = {
    {BWAPI::UnitTypes::Zerg_Drone, 8},
    {BWAPI::UnitTypes::Zerg_Drone, 10},
    {BWAPI::UnitTypes::Zerg_Drone, 12},
    {BWAPI::UnitTypes::Zerg_Drone, 14},
    {BWAPI::UnitTypes::Zerg_Drone, 16},
    {BWAPI::UnitTypes::Zerg_Spawning_Pool, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 16},
    {BWAPI::UnitTypes::Zerg_Overlord, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 18},
    {BWAPI::UnitTypes::Zerg_Zergling, 20},
    {BWAPI::UnitTypes::Zerg_Zergling, 22},
    {BWAPI::UnitTypes::Zerg_Zergling, 24},
    {BWAPI::UnitTypes::Zerg_Hatchery, 26}
};
    
// 12 hatch on large maps
// https://liquipedia.net/starcraft/12_Hatch_(vs._Terran)
std::initializer_list<std::pair<BWAPI::UnitType, int>> terran_large = {
    {BWAPI::UnitTypes::Zerg_Drone, 8},
    {BWAPI::UnitTypes::Zerg_Drone, 10},
    {BWAPI::UnitTypes::Zerg_Drone, 12},
    {BWAPI::UnitTypes::Zerg_Drone, 14},
    {BWAPI::UnitTypes::Zerg_Drone, 16},
    {BWAPI::UnitTypes::Zerg_Overlord, 18},
    {BWAPI::UnitTypes::Zerg_Drone, 20},
    {BWAPI::UnitTypes::Zerg_Drone, 22},
    {BWAPI::UnitTypes::Zerg_Hatchery, 24},
    {BWAPI::UnitTypes::Zerg_Drone, 22},
    {BWAPI::UnitTypes::Zerg_Drone, 24},
    {BWAPI::UnitTypes::Zerg_Spawning_Pool, 26}
};