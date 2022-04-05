#include "./AdditionalPylonsModule.h"

#include "bwem.h"
#include "BWEB.h"

#include "./Players/Player.h"
#include "./Strategist/Strategist.h"
#include "./Strategist/ScoutEngine/ScoutEngine.h"

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

	PlayerUpgrades::onStart();
	Player::getPlayerInstance().onStart(BWAPI::Broodwar->self()->getRace());
	Player::getEnemyInstance().onStart(BWAPI::Broodwar->enemy()->getRace());
	

	Strategist::getInstance().onStart();
	ScoutEngine::getInstance().onStart();
}
	
void AdditionalPylonsModule::onEnd(bool isWinner) {

}

void AdditionalPylonsModule::onFrame() {
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	Strategist::getInstance().onFrame();
	ScoutEngine::getInstance().onFrame();
	ScoutEngine::getInstance().displayInfo();

	BWEB::Map::draw();

	PlayerUpgrades::onFrame();
	Player::getPlayerInstance().onFrame();

	Player::getPlayerInstance().displayInfo(460);
	Player::getEnemyInstance().displayInfo(560);
	Strategist::getInstance().displayInfo(460);

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	BWAPI::Broodwar->drawTextScreen(0, 50, "Module time: %d ms", duration);
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