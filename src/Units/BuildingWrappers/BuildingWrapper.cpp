#include "./BuildingWrapper.h"
#include "../../Strategist/Strategist.h"

void BuildingWrapper::onFrame() {
	// if the unit is not busy and doesnt have a job
	if (!this->isBusy() && !this->hasOrder()) {
		// get build order from strategist
		auto order = Strategist::getInstance().getUnitOrder(this->type);
		if (order.has_value()) 
			this->buildOrder = order.value();
		else {
			// build order doesn't exist, check upgrade order
			auto upgrade = Strategist::getInstance().getUnitUpgradeOrder(this->type);
			if (upgrade.has_value())
				this->upgradeOrder = upgrade.value();
		}
	}

	if (!this->isBusy()) {
		if (this->buildOrder != BWAPI::UnitTypes::Unknown) {
			if (this->unit->build(this->buildOrder))
				this->buildOrder = BWAPI::UnitTypes::Unknown;
		}
		else if (this->upgradeOrder != BWAPI::UpgradeTypes::Unknown) {
			if (this->unit->upgrade(this->upgradeOrder))
				this->upgradeOrder = BWAPI::UpgradeTypes::Unknown;
		}
	}
}

void BuildingWrapper::displayInfo() {
}

bool BuildingWrapper::hasOrder() {
	return (this->buildOrder == BWAPI::UnitTypes::Unknown) && (this->upgradeOrder == BWAPI::UpgradeTypes::Unknown);
}