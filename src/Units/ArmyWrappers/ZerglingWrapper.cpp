#include "./ZerglingWrapper.h"

void ZerglingWrapper::onFrame() {}

void ZerglingWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "ZerglingWrapper");
}