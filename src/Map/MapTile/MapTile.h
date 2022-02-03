#pragma once

#include <BWAPI.h>
class MapTile
{
	public:
		/*
		MapTile constructor
		*/
		MapTile(BWAPI::TilePosition tilePos, BWAPI::Game* game);
		MapTile() = default;
		~MapTile() = default;

		/*
		Sets bool for if the tile is walkable
		*/
		void setWalkability(bool isWalk) { this->walkability = isWalk; }

		/*
		Sets bool for if the tile is buildable
		*/
		void setBuildability(bool isBuild){ this->buildability = isBuild; }

		/*
		Sets bool for if the tile has creep on it
		*/
		void setHasCreep(bool isCreep) { this->hasCreep = isCreep; }

		/*
		Sets int for when the tile was last seen
		*/
		void setLastFrameSeen(int frameSeen) { this->lastFrameSeen = frameSeen; }

		/*
		Sets int for the altitude of the tile
		*/
		void setAltitude(int elevation) { this->altitude = elevation; }

		/*
		Gets whether or not the tile is walkable
		@returns
			@retval True if the tile is walkable
			@retval False if the tile isn't walkable
		*/
		bool getWalkability() { return walkability; }

		/*
		Gets whether or not the tile is buildable
		@returns
			@retval True if the tile is buildable
			@retval False if the tile isn't buildable
		*/
		bool getBuildability() { return buildability; }

		/*
		Gets whether or not the tile has creep on it
		@returns
			@retval True if the tile has creep
			@retval False if the tile doesn't have creep
		*/
		bool getHasCreep() { return hasCreep; }

		/*
		Gets the last frame this tile was seen
		@returns
			@retval int representing the last frame seen
		*/
		int getLastFrameSeen() { return lastFrameSeen; }

		/*
		Gets the altitude of the tile
		@returns
			@retval int = 0 if low ground
			@retval int = 1 if low ground doodad
			@retval int = 2 if high ground
			@retval int = 3 if high ground doodad
			@retval int = 4 if very high ground
			@retval int = 5 if very high ground doodad
		*/
		int getAltitude() { return altitude; }

	private:
		bool walkability;
		bool buildability;
		bool hasCreep;
		int lastFrameSeen;
		int altitude;
};


