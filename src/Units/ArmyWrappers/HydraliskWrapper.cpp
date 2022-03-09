#include "./HydraliskWrapper.h"

void HydraliskWrapper::onFrame() {}

void HydraliskWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "HydraliskWrapper");
}