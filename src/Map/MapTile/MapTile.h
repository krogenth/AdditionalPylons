#pragma once

#include <BWAPI.h>
class MapTile
{
	public:
		/*
		MapTile constructor
		*/
		MapTile(BWAPI::TilePosition tilePos, BWAPI::Game* game);
		~MapTile() = default;
		/*
		Sets bool for if the tile is walkable
		*/
		void setWalkability(bool isWalk);
		/*
		Sets bool for if the tile is buildable
		*/
		void setBuildability(bool isBuild);
		/*
		Sets bool for if the tile has creep on it
		*/
		void setHasCreep(bool isCreep);
		/*
		Sets int for when the tile was last seen
		*/
		void setLastFrameSeen(int frameSeen);
		/*
		Sets int for the altitude of the tile
		*/
		void setAltitude(int elevation);

		/*
		Gets whether or not the tile is walkable
		@returns
			@retval True if the tile is walkable
			@retval False if the tile isn't walkable
		*/
		bool getWalkability();
		/*
		Gets whether or not the tile is buildable
		@returns
			@retval True if the tile is buildable
			@retval False if the tile isn't buildable
		*/
		bool getBuildability();
		/*
		Gets whether or not the tile has creep on it
		@returns
			@retval True if the tile has creep
			@retval False if the tile doesn't have creep
		*/
		bool getHasCreep();
		/*
		Gets the last frame this tile was seen
		@returns
			@retval int representing the last frame seen
		*/
		int getLastFrameSeen();
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
		int getAltitude();

	private:
		bool walkability;
		bool buildability;
		bool hasCreep;
		int lastFrameSeen;
		int altitude;
		BWAPI::TilePosition tilePosition;
		BWAPI::Game* gameObj;
};


