#include "./AdditionalPylonsModule.h"

#include "bwem.h"
#include "BWEB.h"

#include "./Players/Player.h"
#include "./Strategist/Strategist.h"

void AdditionalPylonsModule::onStart() {
	//	initialize BWEM
	BWEM::Map::Instance().Initialize(BWAPI::BroodwarPtr);
	BWEM::Map::Instance().EnableAutomaticPathAnalysis();

	//	initialize BWEB, must be after BWEM
	BWEB::Map::onStart();
	BWEB::Stations::findStations();
	BWEB::Blocks::findBlocks();

	//	set UserInput Flag so BWAPI::Broodwar->getScreenPosition() will return valid BWAPI::TilePositions for drawing
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
	BWAPI::Broodwar->setCommandOptimizationLevel(1);
	BWAPI::Broodwar->setLocalSpeed(10);
	BWAPI::Broodwar->setFrameSkip(0);

	Player::getPlayerInstance().onStart(BWAPI::Broodwar->self()->getRace());
	Player::getEnemyInstance().onStart(BWAPI::Broodwar->enemy()->getRace());
	PlayerUpgrades::onStart();

	Strategist::getInstance().onStart();
}
	
void AdditionalPylonsModule::onEnd(bool isWinner) {

}

void AdditionalPylonsModule::onFrame() {
Strategist::getInstance().onFrame();

	BWEB::Map::draw();

	Player::getPlayerInstance().onFrame();

	Player::getPlayerInstance().displayInfo(400);
	Player::getEnemyInstance().displayInfo(530);
	Strategist::getInstance().displayInfo(400);
}

void AdditionalPylonsModule::onSendText(std::string text) {

}

void AdditionalPylonsModule::onReceiveText(BWAPI::Player player, std::string text) {

}

void AdditionalPylonsModule::onPlayerLeft(BWAPI::Player player) {

}

void AdditionalPylonsModule::onNukeDetect(BWAPI::Position target) {

}

void AdditionalPylonsModule::onUnitDiscover(BWAPI::Unit unit) {
	this->onUnitCreate(unit);
}

void AdditionalPylonsModule::onUnitEvade(BWAPI::Unit unit) {

}

void AdditionalPylonsModule::onUnitShow(BWAPI::Unit unit) {
	this->onUnitCreate(unit);
}

void AdditionalPylonsModule::onUnitHide(BWAPI::Unit unit) {
	
}

void AdditionalPylonsModule::onUnitCreate(BWAPI::Unit unit) {
	if (unit->getPlayer() == BWAPI::Broodwar->self()) {
		Player::getPlayerInstance().onUnitCreate(unit);
	}
	else if(unit->getPlayer() != BWAPI::Broodwar->neutral()) {
		Player::getEnemyInstance().onUnitCreate(unit);
	}
}

void AdditionalPylonsModule::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getType().isMineralField())
		BWEM::Map::Instance().OnMineralDestroyed(unit);
	else if (unit->getType().isSpecialBuilding())
		BWEM::Map::Instance().OnStaticBuildingDestroyed(unit);
		
	if (unit->getPlayer() == BWAPI::Broodwar->self()) {
		Player::getPlayerInstance().onUnitDestroy(unit);
		Strategist::getInstance().adjustTotalSupply(-unit->getType().supplyProvided());
	}
	else if (unit->getPlayer() != BWAPI::Broodwar->neutral()) {
		Player::getEnemyInstance().onUnitDestroy(unit);
	}
}

void AdditionalPylonsModule::onUnitMorph(BWAPI::Unit unit) {
	if (unit->getPlayer() == BWAPI::Broodwar->self()) {
		Player::getPlayerInstance().onUnitMorph(unit);
	}
	else if (unit->getPlayer() != BWAPI::Broodwar->neutral()) {
		Player::getEnemyInstance().onUnitMorph(unit);
	}
}

void AdditionalPylonsModule::onUnitRenegade(BWAPI::Unit unit) {

}

void AdditionalPylonsModule::onSaveGame(std::string gameName) {

}

void AdditionalPylonsModule::onUnitComplete(BWAPI::Unit unit) {

}