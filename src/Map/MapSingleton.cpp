#include "MapSingleton.h"

static auto& map = MapSingleton::getInstance();

void MapSingleton::onStart(BWAPI::Game* game) {
	this->m_size = BWAPI::TilePosition(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
    for (int y = 0; y < this->m_size.y; ++y)
    {
        for (int x = 0; x < this->m_size.y; ++x)
        {
            BWAPI::TilePosition current = BWAPI::TilePosition(x, y);
            this->m_map.push_back(MapTile::MapTile(current, game));
        }
    }
}

const BWAPI::TilePosition& MapSingleton::getSize()
{
    return this->m_size;
}

const MapTile& MapSingleton::getTile(int x, int y) {
    int vecPos = this->toOneD(x,y);
    return this->m_map.at(vecPos);
}

const std::vector<MapTile>& MapSingleton::getTiles() {
    return this->m_map;
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

void MapSingleton::draw() {
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
                int vecPos = (MapSingleton::toOneD(x, y));
                BWAPI::Color color;
                if (m_map.at(vecPos).getHasCreep()) this->drawTile(tilePos, BWAPI::Colors::Red);
                else if (m_map.at(vecPos).getWalkability() && m_map.at(vecPos).getBuildability()) this->drawTile(tilePos, BWAPI::Colors::Yellow);
                else if (m_map.at(vecPos).getWalkability()) this->drawTile(tilePos, BWAPI::Colors::Blue);
                else if (m_map.at(vecPos).getBuildability()) this->drawTile(tilePos, BWAPI::Colors::Green);
            }
        }
    }
}

