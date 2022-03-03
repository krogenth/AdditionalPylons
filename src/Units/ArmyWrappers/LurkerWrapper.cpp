#include "./LurkerWrapper.h"

void LurkerWrapper::onFrame() {}

void LurkerWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "LurkerWrapper");
}