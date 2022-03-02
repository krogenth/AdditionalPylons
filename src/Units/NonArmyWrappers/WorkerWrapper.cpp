#include "./WorkerWrapper.h"
#include "../../Strategist/Strategist.h"
#include "BWEB.h"

void WorkerWrapper::onFrame()
{
    // if not busy or working
    if (this->currJob == Jobs::None || !this->isBusy())
    { // get command from strategist
        auto order = Strategist::getInstance().getUnitOrder(this->type);

        if (order.has_value()) // if build order recieved
        {                      // store results
            this->buildOrder = order.value();
            this->currJob = Jobs::MorphIntoNewBuildings;
        }
    }

    // not busy + build order
    if (this->buildOrder != BWAPI::UnitTypes::Unknown && !this->isBusy())
    { // get closest bweb block to the starting location of the right size
        auto tiles = BWEB::Blocks::getClosestBlock(BWAPI::Broodwar->self()->getStartLocation())->getPlacements(this->buildOrder);
        if (tiles.size() > 0)
        { // tileset not empty
            // attempt to build, if fail
            if (!this->unit->build(this->buildOrder, *(tiles.begin())))
            { // move to the tile
                this->unit->move(BWAPI::Position(*(tiles.begin())));
            }
        }
    }
    else if (this->currJob == Jobs::None)
    { // if slacking, go dig
        BWAPI::Unit closest = nullptr;

        for (const auto &i : BWAPI::Broodwar->getMinerals())
        { // find closest mineral
            if ((closest != nullptr && this->unit->getDistance(i) < this->unit->getDistance(closest)) || closest == nullptr)
            {
                closest = i;
            }
        }
        if (closest && this->unit->gather(closest))
        { // ROCK AND STONE
            currJob = Jobs::MineMinerals;
        }
    }
}

void WorkerWrapper::displayInfo()
{
    BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "WorkerWrapper");
    if (!this->queue.empty())
        BWAPI::Broodwar->drawLineMap(BWAPI::Position(this->unit->getTilePosition()), BWAPI::Position(this->queue.back()), BWAPI::Colors::White);
}