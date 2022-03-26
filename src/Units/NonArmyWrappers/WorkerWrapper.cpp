#include "./WorkerWrapper.h"
#include <algorithm>
#include "../../Players/Player.h"
#include "../../Strategist/Strategist.h"
#include "BWEB.h"

void WorkerWrapper::onFrame() {
    // if not busy or working
    if (this->currJob == Jobs::None || !this->isBusy()) {  // get command from strategist
        auto order = Strategist::getInstance().getUnitOrder(this->type);

        if (order.has_value())  // if build order recieved
        {                       // store results
            this->buildOrder = order.value();
            this->currJob = Jobs::MorphIntoNewBuildings;
            if(!this->currRes) {
                Player::getPlayerInstance().adjustResourceWorkerCount(currRes, -1);
                currRes = nullptr;
            }
        }
    }

    // not busy + build order
    if (this->buildOrder != BWAPI::UnitTypes::Unknown && !this->isBusy()) {  // get closest BWEB block to the starting location of the right size
        BWAPI::TilePosition tile = BWAPI::TilePositions::Invalid;
        if (this->buildOrder == BWAPI::UnitTypes::Zerg_Extractor) {
            for (const auto& key : Player::getPlayerInstance().getBuildingAreas()) {
                for (const auto& geyser : key->Geysers()) {
                    if(BWAPI::Broodwar->canBuildHere(geyser->TopLeft(), this->buildOrder)){
                        tile = geyser->TopLeft();
                        break;
                    }
                }
                if(tile != BWAPI::TilePositions::Invalid) {
                    break;
                }
            }
        } else {
            tile = getBlockOfSize(this->buildOrder, BWAPI::Broodwar->self()->getStartLocation());
        }

        if (tile != BWAPI::TilePositions::Invalid) {
            if (!this->unit->build(this->buildOrder, tile)) {  // build failed, move to center of build location
                BWAPI::TilePosition offset(this->buildOrder.tileWidth() / 2, this->buildOrder.tileHeight() / 2);
                this->unit->move(BWAPI::Position(tile + offset));
            }
        }
    } else if (this->currJob == Jobs::None) {
        BWEM::Ressource* firstResOption = nullptr;
        BWEM::Ressource* secondResOption = nullptr;

        // check the ratio of gas to mineral workers we have, if there are too few, try to find a geyser to extract from
        // use some arbitrary value for what the expected ratio should be, this is definitely too high
        if(Player::getPlayerInstance().getGasToMineralWorkerRatio() < 0.25f) {
            firstResOption = Player::getPlayerInstance().getClosestGeyser(this->unit->getPosition());
            secondResOption = Player::getPlayerInstance().getClosestMineral(this->unit->getPosition());
        } else {
            firstResOption = Player::getPlayerInstance().getClosestMineral(this->unit->getPosition());
            secondResOption = Player::getPlayerInstance().getClosestGeyser(this->unit->getPosition());
        }

        // attempt to assign the worker to gather from either a mineral or geyser
        if (firstResOption && this->unit->gather(firstResOption->Unit())) {
            this->currJob = firstResOption->Unit()->getType().isMineralField() ? Jobs::MineMinerals : Jobs::ExtractGas;
            Player::getPlayerInstance().adjustResourceWorkerCount(firstResOption, 1);
            this->currRes = firstResOption;
        } else if (secondResOption && this->unit->gather(secondResOption->Unit())) {
            this->currJob = secondResOption->Unit()->getType().isMineralField() ? Jobs::MineMinerals : Jobs::ExtractGas;
            Player::getPlayerInstance().adjustResourceWorkerCount(secondResOption, 1);
            this->currRes = secondResOption;
        }
    }
}

BWAPI::TilePosition WorkerWrapper::getBlockOfSize(BWAPI::UnitType type, BWAPI::TilePosition pos) {
    std::vector<BWEB::Block> blocks = BWEB::Blocks::getBlocks();

    std::sort(blocks.begin(), blocks.end(),
              [pos](BWEB::Block a, BWEB::Block b) {
                  return a.getTilePosition().getApproxDistance(pos) < b.getTilePosition().getApproxDistance(pos);
              });

    for (auto& b : blocks) {
        auto placements = b.getPlacements(type);
        if (placements.size() && BWAPI::Broodwar->canBuildHere(*placements.begin(), type, this->unit)) {
            return *placements.begin();
        }
    }
    return BWAPI::TilePositions::Invalid;
}

void WorkerWrapper::displayInfo() {
    BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "WorkerWrapper");
    if (!this->queue.empty())
        BWAPI::Broodwar->drawLineMap(BWAPI::Position(this->unit->getTilePosition()), BWAPI::Position(this->queue.back()), BWAPI::Colors::White);
}