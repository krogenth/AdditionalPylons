#include "./AdditionalPylonsModule.h"

#include "./Map/MapSingleton.h"

static auto& map = MapSingleton::getInstance();

void AdditionalPylonsModule::onStart() {
	
	BWAPI::Broodwar->setLocalSpeed(10);
	BWAPI::Broodwar->setFrameSkip(0);
	//	set UserInput Flag so BWAPI::Broodwar->getScreenPosition() will return valid BWAPI::TilePositions for drawing
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
	map.onStart(BWAPI::BroodwarPtr);

}

void AdditionalPylonsModule::onEnd(bool isWinner) {

	

}

void AdditionalPylonsModule::onFrame() {

	map.draw();

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



}

void AdditionalPylonsModule::onUnitEvade(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitShow(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitHide(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitCreate(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitDestroy(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitMorph(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitRenegade(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onSaveGame(std::string gameName) {



}

void AdditionalPylonsModule::onUnitComplete(BWAPI::Unit unit) {



}