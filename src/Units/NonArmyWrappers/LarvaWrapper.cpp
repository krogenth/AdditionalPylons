#include "./LarvaWrapper.h"

void LarvaWrapper::onFrame() {

}

void LarvaWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "LarvaWrapper");
}