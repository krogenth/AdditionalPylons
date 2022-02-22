#include "./WorkerWrapper.h"

void WorkerWrapper::onFrame() {
    BWAPI::Unit closest = nullptr;
    int smallestDistance = INT32_MAX;
    if (this->currJob == Jobs::None) {
        for (const auto& i : BWAPI::Broodwar->getMinerals()) {
            if (this->unit->getDistance(i) < smallestDistance) {
                smallestDistance = this->unit->getDistance(i);
                closest = i;
            }
        }
        if (closest)
        {
            this->unit->gather(closest);
            currJob = Jobs::MineMinerals;
        }
    }
}

void WorkerWrapper::displayInfo() {
    BWAPI::Broodwar->drawTextMap(this->unit->getPosition(), "WorkerWrapper");
    if (!this->queue.empty())
        BWAPI::Broodwar->drawLineMap(BWAPI::Position(this->unit->getTilePosition()), BWAPI::Position(this->queue.back()), BWAPI::Colors::White);
}