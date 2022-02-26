#include "./BuildingWrapper.h"
#include "../../Strategist/Strategist.h"

void BuildingWrapper::onFrame()
{
	//if the unit is not busy and doesnt have a job
	if (!this->isBusy() && this->buildOrder == BWAPI::UnitTypes::Unknown)
	{//get order from strategist
		auto order = Strategist::getInstance().getUnitOrder(this->type);
		if (order.has_value()) // if build order recieved
		{					   // store results
			this->buildOrder = order.value();
		}
	}

	//if there's an order and it is not busy
	if (this->buildOrder != BWAPI::UnitTypes::Unknown && !this->isBusy())
	{//build
		unit->build(this->buildOrder);
	}
}

void BuildingWrapper::displayInfo()
{
	BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "BuildingWrapper");
}