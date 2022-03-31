#pragma once

#include <BWAPI.h>
#include <bwem.h>
#include <BWEB.h>

namespace lambdas::map {
	const std::function<bool (const BWAPI::TilePosition&)> getWalkableLambda = [](const BWAPI::TilePosition& pos) -> bool {
		BWAPI::WalkPosition walkPos = BWAPI::WalkPosition(pos);
		for (int i = 0; i < (BWAPI::TILEPOSITION_SCALE / BWAPI::WALKPOSITION_SCALE); i++) {
			walkPos = BWAPI::WalkPosition(walkPos.x + (i % 2), walkPos.y + (i / 2));
			if (!BWAPI::Broodwar->isWalkable(walkPos))
				return false;
		}
		return true;
	};
}