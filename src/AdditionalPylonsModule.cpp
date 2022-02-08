#include "./AdditionalPylonsModule.h"
#include "./Players/Player.h"

void AdditionalPylonsModule::onStart() {
	player.onStart(BWAPI::Broodwar->self()->getRace());
	enemy.onStart(BWAPI::Broodwar->enemy()->getRace());
	BWAPI::Broodwar->setLocalSpeed(10);
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
}

void AdditionalPylonsModule::onEnd(bool isWinner) {

	

}

void AdditionalPylonsModule::onFrame() {
	player.displayInfo(400);
	enemy.displayInfo(530);
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
	onUnitCreate(unit);
}

void AdditionalPylonsModule::onUnitEvade(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitShow(BWAPI::Unit unit) {
	onUnitCreate(unit);
}

void AdditionalPylonsModule::onUnitHide(BWAPI::Unit unit) {
	


}

void AdditionalPylonsModule::onUnitCreate(BWAPI::Unit unit) {
	if (unit->getPlayer() == BWAPI::Broodwar->self()) {
		player.onUnitCreate(unit);
	}
	else if(unit->getPlayer() != BWAPI::Broodwar->neutral()){
		enemy.onUnitCreate(unit);
	}
}

void AdditionalPylonsModule::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getPlayer() == BWAPI::Broodwar->self()) {
		player.onUnitDestroy(unit);
	}
	else if (unit->getPlayer() != BWAPI::Broodwar->neutral()) {
		enemy.onUnitDestroy(unit);
	}
}

void AdditionalPylonsModule::onUnitMorph(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitRenegade(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onSaveGame(std::string gameName) {



}

void AdditionalPylonsModule::onUnitComplete(BWAPI::Unit unit) {



}