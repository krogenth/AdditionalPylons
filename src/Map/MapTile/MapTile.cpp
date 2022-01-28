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

MapTile::MapTile(BWAPI::TilePosition tilePos, BWAPI::Game* game) {
	buildability = game->isBuildable(tilePos);
	altitude = game->getGroundHeight(tilePos);
	hasCreep = game->hasCreep(tilePos);
    walkability = walkabilityLambda(tilePos, game);
    if (BWAPI::Broodwar->isVisible(tilePos)) {
        lastFrameSeen = BWAPI::Broodwar->getFrameCount();
    }
    else
        lastFrameSeen = -1;
}
