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
        }
    }

    // not busy + build order
    if (this->buildOrder != BWAPI::UnitTypes::Unknown && !this->isBusy()) {  // get closest bweb block to the starting location of the right size
        if(!currRes){
            Player::getPlayerInstance().AdjResWorkCount(currRes, -1);
            currRes = nullptr;
        }
        BWAPI::TilePosition tile = BWAPI::TilePositions::Invalid;
        if (this->buildOrder == BWAPI::UnitTypes::Zerg_Extractor) {
            for (const auto& key : Player::getPlayerInstance().getBuildingAreas()) {
                for (const auto& geyser : key->Geysers()) {
                    if(BWAPI::Broodwar->canBuildHere(geyser->TopLeft(), this->buildOrder)){
                        tile = geyser->TopLeft();
                        break;
                    }
                }
                if(tile != BWAPI::TilePositions::Invalid){
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
    } else if (this->currJob == Jobs::None) {  // if slacking, go dig rew
        BWEM::Ressource* res = nullptr;
        if(Player::getPlayerInstance().MineralGasRatio() > 1.0){
            res = Player::getPlayerInstance().getClosestGeyser(this->unit->getPosition());
            if(!res){
                res = Player::getPlayerInstance().getClosestMineral(this->unit->getPosition());
            }
        }else{
            res = Player::getPlayerInstance().getClosestMineral(this->unit->getPosition());
            if(!res){
                res = Player::getPlayerInstance().getClosestGeyser(this->unit->getPosition());
            }
        }
        if(res){
            if(this->unit->gather(res->Unit())){
                if(res->Unit()->getType().isMineralField()){
                    this->currJob = Jobs::MineMinerals;
                }else{
                    this->currJob = Jobs::ExtractGas;
                }
                Player::getPlayerInstance().AdjResWorkCount(res, 1);
                currRes = res;
            }
        }
        // BWAPI::Unit closest = nullptr;

        // for (const auto& i : BWAPI::Broodwar->getMinerals()) {  // find closest mineral
        //     if ((closest != nullptr && this->unit->getDistance(i) < this->unit->getDistance(closest)) || closest == nullptr) {
        //         closest = i;
        //     }
        // }
        // if (closest && this->unit->gather(closest)) {  // ROCK AND STONE
        //     currJob = Jobs::MineMinerals;
        // }
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