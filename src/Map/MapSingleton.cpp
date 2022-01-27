#include "MapSingleton.h"

void MapSingleton::onStart() {
	MapSingleton::m_size = BWAPI::TilePosition(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
    for (int x = 0; x < MapSingleton::m_size.x; ++x)
    {
        for (int y = 0; y < MapSingleton::m_size.y; ++y)
        {
            BWAPI::TilePosition current = BWAPI::TilePosition(x, y);
            MapSingleton::m_map.push_back(MapTile::MapTile(current, MapSingleton::gameObj));
        }
    }
}

const BWAPI::TilePosition& MapSingleton::getSize()
{
    return MapSingleton::m_size;
}

const BWAPI::TilePosition& MapSingleton::getTile(int x, int y) {
    return BWAPI::TilePosition(x, y);
}

const std::vector<BWAPI::TilePosition>& MapSingleton::getTiles() {
    std::vector<BWAPI::TilePosition> retVec;
    for (int x = 0; x < MapSingleton::m_size.x; ++x)
    {
        for (int y = 0; y < MapSingleton::m_size.y; ++y)
        {
            BWAPI::TilePosition current = BWAPI::TilePosition(x, y);
            retVec.push_back(current);           
        }
    }
    return retVec;
}
/*This is using the draw functions found in davechurchill's STARTcraft https://github.com/davechurchill/STARTcraft */
void MapSingleton::drawTile(BWAPI::TilePosition tile, const BWAPI::Color& color){
    const int padding = 2;
    const int px = tile.x * 32 + padding;
    const int py = tile.y * 32 + padding;
    const int d = 32 - 2 * padding;

    BWAPI::Broodwar->drawLineMap(px, py, px + d, py, color);
    BWAPI::Broodwar->drawLineMap(px + d, py, px + d, py + d, color);
    BWAPI::Broodwar->drawLineMap(px + d, py + d, px, py + d, color);
    BWAPI::Broodwar->drawLineMap(px, py + d, px, py, color);
}

void MapSingleton::draw(){
    const BWAPI::TilePosition screen(BWAPI::Broodwar->getScreenPosition());
    const int sx = screen.x;
    const int sy = screen.y;
    const int ex = sx + 20;
    const int ey = sy + 15;

    for (int x = sx; x < ex; ++x)
    {
        for (int y = sy; y < ey; y++)
        {
            const BWAPI::TilePosition tilePos(x, y);
            if (!tilePos.isValid()) { continue; }

            if (true)
            {
                int vecPos=(MapSingleton::toOneD(x,y));
                BWAPI::TilePosition tile = BWAPI::TilePosition(x, y);
                BWAPI::Color color;
                //blue
                if (m_map.at(vecPos).getWalkability()) { color = BWAPI::Color(0, 0, 255); }
                //green
                if (m_map.at(vecPos).getBuildability()) { color = BWAPI::Color(0, 255, 0); }
                //pink
                if (m_map.at(vecPos).getWalkability() && m_map.at(vecPos).getBuildability()) { color = BWAPI::Color(255,0,255); }
                //yellow
                if (m_map.at(vecPos).getHasCreep()) { color = BWAPI::Color(255, 255, 0); }
               
                drawTile(tile, color);
            }
        }
    }

