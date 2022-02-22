#include "./BuildingWrapper.h"

void BuildingWrapper::onFrame() {

}

void BuildingWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), (this->isBusy()) ? "Busy" : "Idle");
}