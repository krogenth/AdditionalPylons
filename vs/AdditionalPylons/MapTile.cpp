#include "MapTile.h"

auto walkabilityLambda = [](const BWAPI::TilePosition& pos, const BWAPI::Game* game) {
    BWAPI::WalkPosition walkPos = BWAPI::WalkPosition(pos);
    for (int i = 0; i < 4; i++) {
        walkPos = BWAPI::WalkPosition(walkPos.x + (i % 2), walkPos.y + (i / 2));
        if (!game->isWalkable(walkPos))
            return false;
    }
    return true;
};

MapTile::MapTile(BWAPI::TilePosition tilePos, BWAPI::Game* game)
{
	tilePosition = tilePos;
    gameObj = game;
	buildability = BWAPI::Broodwar->isBuildable(tilePosition);
	altitude = BWAPI::Broodwar->getGroundHeight(tilePosition);
	hasCreep = BWAPI::Broodwar->hasCreep(tilePosition);
    walkability = walkabilityLambda(tilePos, gameObj);
    if (BWAPI::Broodwar->isVisible(tilePos)) {
        lastFrameSeen = BWAPI::Broodwar->getFrameCount();
    }
    else { lastFrameSeen = -1; }
    
}

void MapTile::setWalkability(bool isWalk) {walkability = isWalk; }
void MapTile::setBuildability(bool isBuild) { buildability = isBuild; }
void MapTile::setHasCreep(bool isCreep) { hasCreep = isCreep; }
void MapTile::setLastFrameSeen(int frameSeen) { lastFrameSeen = frameSeen; }
void MapTile::setAltitude(int elevation) { altitude = elevation; }

bool MapTile::getWalkability() { return walkability; }
bool MapTile::getBuildability() { return buildability; }
bool MapTile::getHasCreep() { return hasCreep; }
int MapTile::getLastFrameSeen() { return lastFrameSeen; }
int MapTile::getAltitude() { return altitude; }

