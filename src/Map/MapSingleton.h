#pragma once

#include <BWAPI.h>
#include "..\AdditionalPylons\MapTile.h"

class MapSingleton
{
    BWAPI::TilePosition m_size;
public:
    MapSingleton(const MapSingleton&) = delete;
    MapSingleton(const MapSingleton&&) = delete;
    MapSingleton operator=(const MapSingleton&) = delete;
    MapSingleton& operator=(const MapSingleton&&) = delete;
    /*
    Returns a reference to the current MapSingleton instance
    @returns
        @retval MapSingleton&
    */
    static MapSingleton& getInstance() {
        static MapSingleton instance;
        return instance;
    }
    void onStart();

private:
    MapSingleton() = default;
    std::map<int,MapTile> m_map;
};

