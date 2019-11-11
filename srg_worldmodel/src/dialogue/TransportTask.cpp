#include "srg/dialogue/TransportTask.h"

#include "srg/SRGWorldModel.h"

#include <srgsim/world/Cell.h>
#include <srgsim/world/Object.h>
#include <srgsim/world/ServiceRobot.h>
#include <srgsim/world/World.h>

namespace srg
{
namespace dialogue
{
bool TransportTask::checkSuccess(SRGWorldModel* wm) const
{
    const srgsim::Cell* cell = wm->sRGSimData.getWorld()->getCell(this->coordinate);
    for (const srgsim::Object* object : cell->getObjects()) {
        if (object->getType() == this->objectType) {
            return true;
        }
    }
    return false;
}

bool TransportTask::foundObject(srg::SRGWorldModel* wm) const
{
    if (!this->objectID) {
        return false;
    }
    const srgsim::Object* object = wm->sRGSimData.getWorld()->getObject(this->objectID);
    return object && object->getType() == this->objectType;
}

bool TransportTask::closeToObject(srg::SRGWorldModel* wm) const
{
    if (!foundObject(wm)) {
        return false;
    }
    const srgsim::Object* object = wm->sRGSimData.getWorld()->getObject(this->objectID);
    const srgsim::Cell* goalCell = object->getCell();
    if (!goalCell) {
        return false;
    }

    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }
    srgsim::Coordinate diff = (goalCell->coordinate - ownCoord.value()).abs();
    if (diff.x < 2 && diff.y < 2) {
        return true;
    }
    return false;
}

bool TransportTask::movedToDestination(srg::SRGWorldModel* wm) const
{
    if (!pickedObject(wm)) {
        return false;
    }
    auto ownCoord = wm->sRGSimData.getOwnPositionBuffer().getLastValidContent();
    if (!ownCoord.has_value()) {
        return false;
    }
    srgsim::Coordinate diff = (this->coordinate - ownCoord.value()).abs();
    if (diff.x < 2 && diff.y < 2) {
        return true;
    }
    return false;
}

bool TransportTask::pickedObject(srg::SRGWorldModel* wm) const
{
    if (!foundObject(wm)) {
        return false;
    }

    const srgsim::ServiceRobot* robot = wm->sRGSimData.getWorld()->getRobot(this->receiverID);
    if (!robot) {
        std::cerr << "[TransportTask::foundObject] World does not know myself!" << std::endl;
        return false;
    }

    return robot->getCarriedObject() && robot->getCarriedObject()->getType() == this->objectType;
}

std::ostream& operator<<(std::ostream& os, const srg::dialogue::TransportTask& obj)
{
    os << "[TransportTask] " << obj.objectType << "(ID: " << obj.objectID << ")"
       << " to " << obj.coordinate;
    //    os << obj.actID;
    //    os << obj.previousActID;
    //    os << obj.senderID;
    //    os << obj.receiverID;
    return os;
}

} // namespace dialogue
} // namespace srg
