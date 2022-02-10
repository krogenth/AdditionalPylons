// Opt for 5 pool on small maps
// https://liquipedia.net/starcraft/4/5_Pool
std::initializer_list<std::pair<BWAPI::UnitType, int>> unknown_smallest = {
    {BWAPI::UnitType::Zerg_Drone, 8},
    {BWAPI::UnitType::Zerg_Spawning_Pool, 10},
    {BWAPI::UnitType::Zerg_Drone, 8},
    {BWAPI::UnitType::Zerg_Drone, 10},
    {BWAPI::UnitType::Zerg_Zergling, 12},
    {BWAPI::UnitType::Zerg_Zergling, 14},
    {BWAPI::UnitType::Zerg_Zergling, 16},
    {BWAPI::UnitType::Zerg_Overlord, 18}
}

// 9 pool on medium maps
// https://liquipedia.net/starcraft/9_Pool_(vs._Protoss)
std::initializer_list<std::pair<BWAPI::UnitType, int>> unknown_medium = {
    {BWAPI::UnitType::Zerg_Drone, 8},
    {BWAPI::UnitType::Zerg_Drone, 10},
    {BWAPI::UnitType::Zerg_Drone, 12},
    {BWAPI::UnitType::Zerg_Drone, 14},
    {BWAPI::UnitType::Zerg_Drone, 16},
    {BWAPI::UnitType::Zerg_Spawning_Pool, 18},
    {BWAPI::UnitType::Zerg_Drone, 16},
    {BWAPI::UnitType::Zerg_Overlord, 18},
    {BWAPI::UnitType::Zerg_Drone, 18},
    {BWAPI::UnitType::Zerg_Zergling, 20},
    {BWAPI::UnitType::Zerg_Zergling, 22},
    {BWAPI::UnitType::Zerg_Zergling, 24},
    {BWAPI::UnitType::Zerg_Zergling, 26}
}
    
// 12 hatch on large maps
// https://liquipedia.net/starcraft/12_Hatch_(vs._Protoss)
std::initializer_list<std::pair<BWAPI::UnitType, int>> unknown_large = {
    {BWAPI::UnitType::Zerg_Drone, 8},
    {BWAPI::UnitType::Zerg_Drone, 10},
    {BWAPI::UnitType::Zerg_Drone, 12},
    {BWAPI::UnitType::Zerg_Drone, 14},
    {BWAPI::UnitType::Zerg_Drone, 16},
    {BWAPI::UnitType::Zerg_Overlord, 18},
    {BWAPI::UnitType::Zerg_Drone, 18},
    {BWAPI::UnitType::Zerg_Drone, 20},
    {BWAPI::UnitType::Zerg_Drone, 22},
    {BWAPI::UnitType::Zerg_Hatchery, 24},
    {BWAPI::UnitType::Zerg_Spawning_Pool, 22}
}