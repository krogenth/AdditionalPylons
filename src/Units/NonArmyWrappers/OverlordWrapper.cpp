#include "./OverlordWrapper.h"

void OverlordWrapper::onFrame() {}

void OverlordWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "OverlordWrapper");
}