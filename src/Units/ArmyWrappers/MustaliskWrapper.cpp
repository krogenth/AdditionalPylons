#include "./MutaliskWrapper.h"

void MutaliskWrapper::onFrame() {}

void MutaliskWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "MutaliskWrapper");
}