#pragma once
#include <set>

#include <BWAPI.h>

class ScoutEngine {
public:
	ScoutEngine(const ScoutEngine&) = delete;
    ScoutEngine(const ScoutEngine&&) = delete;
    ScoutEngine operator=(const ScoutEngine&) = delete;
    ScoutEngine& operator=(const ScoutEngine&&) = delete;

	static ScoutEngine& getInstance() {
        static ScoutEngine instance;
        return instance;
    }

	void onStart();
	void onFrame();

	/*
	Returns the next base location to scout to find the enemy
	Returns BWAPI::TilePositions::Invalid if there is no base to scout
	@returns
		@retval BWAPI::TilePosition of the base to start scouting towards
	*/
	BWAPI::TilePosition getNextBaseToScout();

	void displayInfo();

private:
	ScoutEngine() = default;

	std::vector<std::pair<BWAPI::TilePosition, int>> startLocationFrameVisitibilityMap;
	BWAPI::Unit scout = nullptr;
};