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
	if (this->unit->getLastCommand().getTarget()) {
		BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), this->unit->getLastCommand().getTarget()->getPosition(), BWAPI::Colors::White);
	}
	else if (this->unit->getLastCommand().getTargetPosition().isValid()) {
		BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), this->unit->getLastCommand().getTargetPosition(), BWAPI::Colors::White);
	}
	else if (this->unit->getLastCommand().getTargetTilePosition().isValid()) {
		BWAPI::Broodwar->drawLineMap(this->unit->getPosition(), BWAPI::Position(this->unit->getLastCommand().getTargetTilePosition()), BWAPI::Colors::White);
	}
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "LarvaWrapper");
}