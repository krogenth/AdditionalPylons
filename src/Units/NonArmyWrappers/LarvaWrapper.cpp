#include "./LarvaWrapper.h"

#include "../../Strategist/Strategist.h"

void LarvaWrapper::onFrame() {
	std::optional<BWAPI::UnitType> order;
	if (this->buildOrder == BWAPI::UnitTypes::Unknown) {
		order = Strategist::getInstance().getUnitOrder(this->type);
		if (order.has_value())
			this->buildOrder = order.value();
	}

	if (this->buildOrder != BWAPI::UnitTypes::Unknown)
		this->unit->build(this->buildOrder);
}

void LarvaWrapper::displayInfo() {
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "LarvaWrapper");
}