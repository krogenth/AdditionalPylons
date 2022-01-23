#pragma once

#include <BWAPI.h>

namespace Placement {

	/*
	Attempts to find the nearest open position to the player starting position on the map
	@returns
		@retval BWAPI::TilePositions:Invalid if there was no valid position in the map
		@retval A BWAPI::TilePosition nearest to the BWAPI::Broodwar->self() starting location
	*/
	BWAPI::TilePosition getOpenPlacement(const BWAPI::UnitType& type);

};