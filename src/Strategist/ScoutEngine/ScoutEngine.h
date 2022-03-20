#pragma once
#include <deque>

#include <BWAPI.h>

class ScoutEngine {
public:
	ScoutEngine() = default;

	void onStart();
	void onFrame();
	void setScout(BWAPI::Unit unit);
	void onUnitDestroy(BWAPI::Unit unit);

	/*
	Returns the next base location to scout to find the enemy
	Returns BWAPI::TilePositions::Invalid if there is no base to scout
	@returns
		@retval BWAPI::TilePosition of the base to start scouting towards
	*/
	BWAPI::TilePosition getNextBaseToScout();

	/*
	Returns if we currently have a unit assigned to scout
	@returns
		@retval bool check on if a BWAPI::Unit is assigned to scout
	*/
	bool haveScout() { return this->scout != nullptr; }
private:
	std::deque<BWAPI::TilePosition> startingLocations;
	BWAPI::Unit scout = nullptr;
};