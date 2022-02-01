#include "./AdditionalPylonsModule.h"
#include "./Player.h"

Player enemy;
Player player;

void AdditionalPylonsModule::onStart() {
	player.onStart();
	enemy.onStart();
}

void AdditionalPylonsModule::onEnd(bool isWinner) {

	

}

void AdditionalPylonsModule::onFrame() {



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
	if (unit->getPlayer() != BWAPI::Broodwar->self())
		player.onUnitDiscover(unit);
	else {
		enemy.onUnitDiscover(unit);
	}
}

void AdditionalPylonsModule::onUnitEvade(BWAPI::Unit unit) {



}

void AdditionalPylonsModule::onUnitShow(BWAPI::Unit unit) {
	onUnitCreate(unit);
}

void AdditionalPylonsModule::onUnitHide(BWAPI::Unit unit) {
	


}

void AdditionalPylonsModule::onUnitCreate(BWAPI::Unit unit) {
	if (unit->getPlayer() != BWAPI::Broodwar->self()) {
		enemy.onUnitCreate(unit);
	}
	else {
		player.onUnitCreate(unit);
	}
}

void AdditionalPylonsModule::onUnitDestroy(BWAPI::Unit unit) {
	if (unit->getPlayer() != BWAPI::Broodwar->self()) {
		enemy.onUnitDestroy(unit);
	}
	else {
		player.onUnitDestroy(unit);
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