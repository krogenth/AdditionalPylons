#pragma once

#include <BWAPI.h>
#include ".\MapTile\MapTile.h"

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

    void onStart(BWAPI::Game* game);

    /*
    Returns the size of the map
    @returns
        @retval BWAPI::TilePosition m_size
    */
    const BWAPI::TilePosition& getSize() { return this->m_size; }

    /*
    Returns a reference to a specific tile
    @returns
        @retval MapTile&
    */
    const MapTile& getTile(int x, int y) { return this->m_map.at(this->toOneD(x, y)); }

    /*
    Returns a reference to all tiles
    @returns
        @retval std::vector<MapTile>&
    */
    const std::vector<MapTile>& getTiles() { return this->m_map; }

    /*
    Draws all tiles on screen
    */
    void draw();

private:
    MapSingleton() = default;

    /*
    Returns an int 1d representation of a 2d point
    @returns
        @retval int 
    */
    int toOneD(int x, int y) { return x + (m_size.x * y); }

    /*
    Draws a tile with the chosen color
    */
    void drawTile(BWAPI::TilePosition tile, const BWAPI::Color& color);
    
    std::vector<MapTile> m_map;
};

