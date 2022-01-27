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
    void onStart();
    /*
    Returns the size of the map
    @returns
        @retval BWAPI::TilePosition m_size
    */
    const BWAPI::TilePosition& getSize();
    /*
    Returns a reference to a specific tile
    @returns
        @retval MapTile&
    */
    const MapTile& getTile(int x, int y);
    /*
    Returns a reference to all tiles
    @returns
        @retval std::vector<MapTile>&
    */
    const std::vector<MapTile>& getTiles();
    /*
    Draws all tiles on screen
    */
    void draw();
    
private:
    /*
    Returns an int 1d representation of a 2d point
    @returns
        @retval int 
    */
    int toOneD(int x, int y) { return x + (m_size.x * y); };
    /*
    Draws a tile with the chosen color
    */
    void drawTile(BWAPI::TilePosition tile, const BWAPI::Color& color);
    MapSingleton() = default;
    BWAPI::Game* gameObj = BWAPI::BroodwarPtr;
    std::vector<MapTile> m_map;
};

